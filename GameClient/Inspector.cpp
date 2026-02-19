#include "pch.h"
#include "Inspector.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "TransformUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "CameraUI.h"
#include "MeshRendererUI.h"

Inspector::Inspector()
	: EditorUI("Inspector")
{
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSizeAsChild(Vec2(0.f, 250.f));
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM].Get());

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSizeAsChild(Vec2(0.f, 200.f));
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D].Get());

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSizeAsChild(Vec2(0.f, 150.f));
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D].Get());

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSizeAsChild(Vec2(0.f, 300.f));
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA].Get());

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDERER] = new MeshRendererUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDERER]->SetSizeAsChild(Vec2(0.f, 300.f));
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDERER].Get());

	SetTargetObject(SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"Player"));
}

Inspector::~Inspector()
{
}

void Inspector::Tick_UI()
{
	if (m_targetObj == nullptr)
		return;

	wstring name = m_targetObj->GetName();
	string strName = string(name.begin(), name.end());
	ImGui::Button(strName.c_str());

	ImGui::Separator();
}

void Inspector::SetTargetObject(Ptr<GameObject> _obj)
{
	m_targetObj = _obj;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComUI[i] == nullptr)
			continue;

		m_arrComUI[i]->SetTarget(m_targetObj);
	}
}
