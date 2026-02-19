#include "pch.h"
#include "AScene.h"
#include "GameObject.h"

AScene::AScene()
	: Asset(ASSET_TYPE::SCENE), m_matrix{}
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
					queue.push_back(vecChild[i]);
				}
			}
		}
	}
	return nullptr;
}
