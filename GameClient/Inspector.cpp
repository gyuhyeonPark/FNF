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
	CreateChildUI();
	SetTargetObject(nullptr);
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
	
	if (strName.empty())
		strName = "No Name";
	
	ImGui::Button(strName.c_str());

	ImGui::Separator();
}

void Inspector::SetTargetObject(Ptr<GameObject> _obj)
{
	// 입력된 게임 오브젝트의 정보를 보여줄 ComponentUI들을 활성화 시킨다.
	m_targetObj = _obj;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComUI[i] == nullptr)
			continue;

		m_arrComUI[i]->SetTarget(m_targetObj);
	}

	// AssetUI를 비활성화한다.
	m_TargetAsset = nullptr;
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (m_arrAssetUI[i] != nullptr)
			m_arrAssetUI[i]->SetActive(false);
	}
}

void Inspector::SetTargetAsset(Ptr<Asset> _Asset)
{
	// ComponentUI들을 비활성화 시킨다.
	SetTargetObject(nullptr);

	// 입력된 에셋 담당 UI를 활성화 시킨다.
	m_TargetAsset = _Asset;
	if (m_TargetAsset == nullptr)
	{
		for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
		{
			m_arrAssetUI[i]->SetActive(false);
		}
	}
	else
	{
		ASSET_TYPE Type = m_TargetAsset->GetType();
		m_arrAssetUI[(UINT)Type]->SetActive(true);
		m_arrAssetUI[(UINT)Type]->SetTargetAsset(m_TargetAsset);
	}
}
