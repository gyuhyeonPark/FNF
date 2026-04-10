#include "pch.h"
#include "EditorUI.h"
#include "EditorManager.h"

EditorUI::EditorUI(const string& uiName)
	: m_active(true), m_parent(nullptr), m_UIName(uiName)
	, m_IsModal(false), m_Separator(true)
{
}

EditorUI::~EditorUI()
{
}

void EditorUI::Tick()
{
	if (m_IsModal)
	{
		string StrKey = m_UIName + m_UIKey;
		ImGui::OpenPopup(StrKey.c_str());

		bool Active = m_active;

		if (ImGui::BeginPopupModal(StrKey.c_str(), &Active, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav))
		{
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

			ImGui::EndPopup();
		}
		else
		{
			SetActive(Active);
		}
	}

	else if (nullptr == m_parent)
	{
		bool Active = m_active;

		string StrKey = m_UIName + m_UIKey;

		ImGui::Begin(StrKey.c_str(), &Active);

		if (m_active != Active)
		{
			SetActive(Active);
		}

		CheckFocus();

		Tick_UI();

		for (size_t i = 0; i < m_childUI.size(); ++i)
		{
			if (m_childUI[i]->IsActive())
			{
				m_childUI[i]->Tick();
			}
		}

		ImGui::End();


	}

	else
	{
		ImGui::BeginChild(m_UIName.c_str(), m_SizeAsChild, 0,
			ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavFocus);

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

		if (m_Separator)
		{
			ImGui::Separator();
		}
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