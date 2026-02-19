#include "pch.h"
#include "ComponentUI.h"
#include "GameObject.h"

ComponentUI::ComponentUI(COMPONENT_TYPE type, const string& name)
	: EditorUI(name), m_comType(type)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::SetTarget(Ptr<GameObject> obj)
{
	m_target = obj;

	if (m_target == nullptr || m_target->GetComponent(m_comType) == nullptr)
		SetActive(false);
	else
		SetActive(true);

}

void ComponentUI::OutputTitle(const string& _title)
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::Button(_title.c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}
