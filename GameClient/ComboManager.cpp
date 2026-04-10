#include "pch.h"
#include "ComboManager.h"
#include "AssetManager.h"

ComboManager::ComboManager()
	: m_currentCombo(0), m_destination(Vec3(0.f, 100.f, 0.f))
	, m_startPosition(Vec3(0.f, -100.f, 0.f)), gravity(9.8f), m_velocity(6.f)
{
	m_comboNumVec.resize(3);
	m_tagSprites.resize(3);
	m_numSprites.resize(10);
}

ComboManager::~ComboManager()
{
}

void ComboManager::Init()
{
	// 관련 sprite 받아오기
	Ptr<GameObject> obj;

	obj = new GameObject;
	obj->AddComponent(new CTransform);
	obj->AddComponent(new CMeshRenderer);
	obj->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	obj->MeshRenderer()->SetMtrl(FIND(AMaterial, L"ComboTag"));
	obj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	m_comboSprite = obj;

	obj = new GameObject;
	obj->AddComponent(new CTransform);
	obj->AddComponent(new CMeshRenderer);
	obj->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	obj->MeshRenderer()->SetMtrl(FIND(AMaterial, L"Shit"));
	obj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	m_tagSprites[(UINT)ComboTagIdx::IDX_SHIT] = obj;

	obj = new GameObject;
	obj->AddComponent(new CTransform);
	obj->AddComponent(new CMeshRenderer);
	obj->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	obj->MeshRenderer()->SetMtrl(FIND(AMaterial, L"Bad"));
	obj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	m_tagSprites[(UINT)ComboTagIdx::IDX_BAD] = obj;

	obj = new GameObject;
	obj->AddComponent(new CTransform);
	obj->AddComponent(new CMeshRenderer);
	obj->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	obj->MeshRenderer()->SetMtrl(FIND(AMaterial, L"Good"));
	obj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	m_tagSprites[(UINT)ComboTagIdx::IDX_GOOD] = obj;

	obj = new GameObject;
	obj->AddComponent(new CTransform);
	obj->AddComponent(new CMeshRenderer);
	obj->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	obj->MeshRenderer()->SetMtrl(FIND(AMaterial, L"Sick"));
	obj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	m_tagSprites[(UINT)ComboTagIdx::IDX_SICK] = obj;

	for (int i = 0; i < 10; i++)
	{
		wstring tag = to_wstring(i);
		obj = new GameObject;
		obj->AddComponent(new CTransform);
		obj->AddComponent(new CMeshRenderer);
		obj->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
		obj->MeshRenderer()->SetMtrl(FIND(AMaterial, L"ComboNum" + tag));
		obj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
		m_numSprites[i] = obj;
	}
}

void ComboManager::Tick()
{
	if (m_isPopStart)
	{
		m_elapsed += DT;

		float yPos = m_velocity * m_elapsed - 0.5f * gravity * pow(m_elapsed, 2);

		Vec3 currentPos = m_tagSprites[(UINT)m_currentTag]->Transform()->GetRelativePosition()
			+ Vec3(0.f, yPos, 0.f);
		m_tagSprites[(UINT)m_currentTag]->Transform()->SetRelativePosition(currentPos);
	}
}

void ComboManager::AddCombo(ComboTagIdx _judge)
{
	m_currentCombo++;

	int pCombo = m_currentCombo;

	for (int i = 0; i < 3; i++)
	{
		int num = pCombo % int(pow(10, i + 1));
		m_comboNumVec[i] = num / pow(10, i);
	}
	m_isPopStart = true;
	m_elapsed = 0.f;
	m_currentTag = _judge;
	m_tagSprites[(UINT)m_currentTag]->Transform()->SetRelativePosition(m_startPosition);
	m_tagSprites[(UINT)m_currentTag]->MeshRenderer()->GetMtrl()->SetScalar(FLOAT_0, 0);
}

void ComboManager::ResetCombo()
{
}
