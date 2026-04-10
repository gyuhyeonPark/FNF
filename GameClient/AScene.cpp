#include "pch.h"
#include "AScene.h"
#include "GameObject.h"

AScene::AScene()
	: Asset(ASSET_TYPE::SCENE), m_matrix{}, m_Changed(false)
{
	for (int i = 0; i < MAX_LAYER; i++)
	{
		m_layerArr[i].m_layerIdx = i;
	}
}

AScene::~AScene()
{
}

void AScene::AddObject(int idx, Ptr<GameObject> obj)
{
	m_layerArr[idx].AddObject(obj);
}

void AScene::Deregister()
{
	for (UINT i = 0; i < MAX_LAYER; i++)
	{
		m_layerArr[i].DeregisterObject();
	}
}

void AScene::Begin()
{
	for (UINT i = 0; i < MAX_LAYER; i++)
	{
		m_layerArr[i].Begin();
	}
}

void AScene::Tick()
{
	for (UINT i = 0; i < MAX_LAYER; i++)
	{
		m_layerArr[i].Tick();
	}
}

void AScene::FinalTick()
{
	for (UINT i = 0; i < MAX_LAYER; i++)
	{
		m_layerArr[i].FinalTick();
	}
}

void AScene::CheckCollisionLayer(UINT layerIdx1, UINT layerIdx2)
{
	UINT row = min(layerIdx1, layerIdx2);
	UINT col = max(layerIdx1, layerIdx2);

	m_matrix[row] ^= (1 << col);
}

void AScene::CheckCollisionLayer(const wstring& layerName1, const wstring& layerName2)
{
	UINT layerIdx1;
	UINT layerIdx2;

	for (UINT i = 0; i < MAX_LAYER; i++)
	{
		if (m_layerArr[i].GetName() == layerName1)
			layerIdx1 = m_layerArr[i].m_layerIdx;

		if (m_layerArr[i].GetName() == layerName2)
			layerIdx2 = m_layerArr[i].m_layerIdx;
	}

	UINT row = min(layerIdx1, layerIdx2);
	UINT col = max(layerIdx1, layerIdx2);

	m_matrix[row] ^= (1 << col);
}

Ptr<GameObject> AScene::FindObjectByName(const wstring& name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<Ptr<GameObject>>& vecParents = m_layerArr[i].GetParentObjects();

		for (size_t i = 0; i < vecParents.size(); i++)
		{
			list<Ptr<GameObject>> queue;
			queue.push_back(vecParents[i]);

			while (!queue.empty())
			{
				Ptr<GameObject> obj = queue.front();
				if (obj->GetName() == name)
					return obj;

				queue.pop_front();

				const vector<Ptr<GameObject>>& vecChild = obj->GetChild();
				for (size_t j = 0; j < vecChild.size(); ++j)
				{
					queue.push_back(vecChild[j]);
				}
			}
		}
	}
	return nullptr;
}

int AScene::Save(const wstring& _FilePath)
{
	// 파일 스트림 커널
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");

	wstring SceneName = GetName();
	SaveWString(pFile, SceneName);

	// 충돌 체크 정보
	fwrite(m_matrix, sizeof(UINT), MAX_LAYER, pFile);

	// 레이어 정보
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		SaveWString(pFile, m_layerArr[i].GetName());

		// 레이어 소속 최상위 부모 오브젝트를 계층구조로 저장.
		const vector<Ptr<GameObject>>& vecParents = m_layerArr[i].GetParentObjects();
		for (const auto& Object : vecParents)
		{
			Object->SaveToSceneFile(pFile);
		}

	}

	fclose(pFile);

	return 0;
}

int AScene::Load(const wstring& _FilePath)
{
	// 파일 스트림 커널
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	wstring SceneName = LoadWString(pFile);
	SetName(SceneName);

	// 충돌 체크 정보
	fread(m_matrix, sizeof(UINT), MAX_LAYER, pFile);

	// 레이어 정보
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 레이어 이름
		wstring LayerName = LoadWString(pFile);
		m_layerArr[i].SetName(LayerName);
	}

	fclose(pFile);
	
	return 0;
}
