#include "pch.h"
#include "EditorUI.h"
#include "EditorManager.h"

EditorUI::EditorUI(const string& uiName)
	: m_active(true), m_parent(nullptr), m_uiName(uiName)
{
}

EditorUI::~EditorUI()
{
}

void EditorUI::Tick()
{
	// 부모는 ImGui::Begin 및 ImGui::End로
	if (m_parent == nullptr)
	{
		bool active = m_active;

		ImGui::Begin(GetUIName().c_str(), &active);

		if (m_active != active)
			SetActive(active);

		CheckFocus();

		Tick_UI();
		
		for (size_t i = 0; i < m_childUI.size(); ++i)
		{
			if (m_childUI[i]->IsActive())
			{
				m_childUI[i]->Tick();
				ImGui::Separator();
			}
		}

		ImGui::End();
	}
	else
	{
		ImGui::BeginChild(GetUIName().c_str(), m_SizeAsChild);

		CheckFocus();

		Tick_UI();
		
		for (size_t i = 0; i < m_childUI.size(); ++i)
		{
			if (m_childUI[i]->IsActive())
			{
				m_childUI[i]->Tick();
				ImGui::Separator();
			}
		}

		ImGui::EndChild();
	}
}

void EditorUI::CheckFocus()
{
	if (ImGui::IsWindowFocused())
		EditorManager::GetInstance()->RegisterFocusedUI(this);
}

Vec2::operator ImVec2() const
{
	return ImVec2(x, y);
}

Vec4::operator ImVec4() const
{
	return ImVec4(x, y, z, w);
}