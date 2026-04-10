#include "pch.h"
#include "MeshRendererUI.h"
#include "CMeshRenderer.h"
#include "GameObject.h"
#include "ListUI.h"
#include "EditorManager.h"
#include "AssetManager.h"


MeshRendererUI::MeshRendererUI()
	: ComponentUI(COMPONENT_TYPE::MESHRENDERER, "MeshRendererUI")
{
}

MeshRendererUI::~MeshRendererUI()
{
}

void MeshRendererUI::Tick_UI()
{
	OutputTitle("MeshRender");
	Ptr<CMeshRenderer> pMeshRender = GetTarget()->MeshRenderer();

	// ====
	// Mesh
	// ====
	ImGui::Text("Mesh");
	ImGui::SameLine(120);

	Ptr<AMesh> pMesh = pMeshRender->GetMesh();
	wstring temp = pMesh->GetKey();
	string MeshKey = string(temp.begin(), temp.end());
	ImGui::InputText("##MeshName", MeshKey.data(), MeshKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// 특정 위젯에서 드래그가 발생했고, 해당 위젯 위에 마우스가 호버링 중인지
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("Content");
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			if (ASSET_TYPE::MESH == pAsset->GetType())
			{
				pMeshRender->SetMesh((AMesh*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", Vec2(20.f, 20.f)))
	{
		// 버튼이 눌리면, 리스트UI 를 찾아서 활성화 시키고, 출력시키고 싶은 문자열을 ListUI 에 등록시킨다.
		Ptr<ListUI> pUI = dynamic_cast<ListUI*>(EditorManager::GetInstance()->FindUI("ListUI").Get());
		assert(pUI.Get());

		pUI->SetUIName("Mesh List");

		vector<wstring> vecMeshNames;
		AssetManager::GetInstance()->GetAssetNames(ASSET_TYPE::MESH, vecMeshNames);
		pUI->AddString(vecMeshNames);
		pUI->AddDelegate(this, (DELEGATE_1)&MeshRendererUI::SelectMesh);
		pUI->SetActive(true);
	}

	// ========
	// Material
	// ========
	ImGui::Text("Material");
	ImGui::SameLine(120);

	Ptr<AMaterial> pMtrl = pMeshRender->GetMaterial();
	temp = pMtrl->GetKey();
	string MtrlKey = string(temp.begin(), temp.end());
	ImGui::InputText("##MtrlName", MtrlKey.data(), MtrlKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// 특정 위젯에서 드래그가 발생했고, 해당 위젯 위에 마우스가 호버링 중인지
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("Content");
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;

			if (ASSET_TYPE::MATERIAL == pAsset->GetType())
			{
				pMeshRender->SetMaterial((AMaterial*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", Vec2(20.f, 20.f)))
	{
		// 버튼이 눌리면, 리스트UI 를 찾아서 활성화 시키고, 출력시키고 싶은 문자열을 ListUI 에 등록시킨다.
		Ptr<ListUI> pUI = dynamic_cast<ListUI*>(EditorManager::GetInstance()->FindUI("ListUI").Get());
		assert(pUI.Get());

		pUI->SetUIName("Material List");

		vector<wstring> vecMtrlName;
		AssetManager::GetInstance()->GetAssetNames(ASSET_TYPE::MATERIAL, vecMtrlName);
		pUI->AddString(vecMtrlName);
		pUI->AddDelegate(this, (DELEGATE_1)&MeshRendererUI::SelectMtrl);
		pUI->SetActive(true);
	}

}

void MeshRendererUI::SelectMesh(DWORD_PTR _ListUI)
{
	Ptr<ListUI> pListUI = ((ListUI*)_ListUI);

	wstring key = wstring(pListUI->GetSelectedString().begin(), pListUI->GetSelectedString().end());

	Ptr<AMesh> pMesh = FIND(AMesh, key);

	GetTarget()->MeshRenderer()->SetMesh(pMesh);
}

void MeshRendererUI::SelectMtrl(DWORD_PTR _ListUI)
{
	Ptr<ListUI> pListUI = ((ListUI*)_ListUI);

	wstring key = wstring(pListUI->GetSelectedString().begin(), pListUI->GetSelectedString().end());

	Ptr<AMaterial> pMtrl = FIND(AMaterial, key);

	GetTarget()->MeshRenderer()->SetMaterial(pMtrl);
}
