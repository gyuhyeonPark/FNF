#include "pch.h"
#include "GameObject.h"
#include "CComboManager.h"
#include "AssetManager.h"
#include "CComboImage.h"
#include "CDotween.h"

CComboManager::CComboManager()
	: m_currentCombo(0), m_startPos(Vec3(0.f, -230.f, -1.f))
{
}

CComboManager::~CComboManager()
{
}

void CComboManager::CreateComboImage(JudgeMent _val)
{
	int combo = m_currentCombo;

	float offsetX = 0.f;

	while (combo != 0)
	{
		int spriteNum = combo % 10;

		GameObject* obj = new GameObject;
		obj->AddComponent(new CTransform);
		obj->AddComponent(new CMeshRenderer);
		obj->AddComponent(new CDotween);
		obj->AddComponent(new CComboImage);

		obj->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));

		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"num%d", spriteNum);
		obj->MeshRenderer()->SetMaterial(LOAD(AMaterial, Buff));
		obj->MeshRenderer()->CreateDynamicMaterial();

		obj->Transform()->SetRelativePosition(m_startPos - Vec3(offsetX, 0.f, 0.f));
		obj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

		CreateObject(obj, 1);

		combo /= 10.f;
		offsetX += 50.f;
	}

	float offsetY = 60.f;

	GameObject* obj = new GameObject;
	obj->AddComponent(new CTransform);
	obj->AddComponent(new CMeshRenderer);
	obj->AddComponent(new CDotween);
	obj->AddComponent(new CComboImage);

	obj->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));

	switch (_val)
	{
	case JudgeMent::E_MISS:
		obj->MeshRenderer()->SetMaterial(LOAD(AMaterial, L"Miss"));
		break;
	case JudgeMent::E_SHIT:
		obj->MeshRenderer()->SetMaterial(LOAD(AMaterial, L"Shit"));
		break;
	case JudgeMent::E_BAD:
		obj->MeshRenderer()->SetMaterial(LOAD(AMaterial, L"Bad"));
		break;
	case JudgeMent::E_GOOD:
		obj->MeshRenderer()->SetMaterial(LOAD(AMaterial, L"Good"));
		break;
	case JudgeMent::E_SICK:
		obj->MeshRenderer()->SetMaterial(LOAD(AMaterial, L"Sick"));
		break;
	}
	obj->MeshRenderer()->CreateDynamicMaterial();

	obj->Transform()->SetRelativePosition(m_startPos + Vec3(0.f, offsetY, 0.f));
	obj->Transform()->SetRelativeScale(Vec3(140.f, 50.f, 1.f));

	CreateObject(obj, 1);
}
