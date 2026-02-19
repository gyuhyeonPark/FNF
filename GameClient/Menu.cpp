#include "pch.h"
#include "Menu.h"
#include "EditorManager.h"

Menu::Menu()
	: EditorUI("Menu")
{
}

Menu::~Menu()
{
}

void Menu::Tick()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Level Save"))
			{

			}
			if (ImGui::MenuItem("Level Load"))
			{

			}
		
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			bool ShowDemo = EditorManager::GetInstance()->IsShowDemo();

			if (ImGui::MenuItem("Demo", nullptr, &ShowDemo, true))
			{
				EditorManager::GetInstance()->ShowDemo(ShowDemo);
			}

			Ptr<EditorUI> pInspector = EditorManager::GetInstance()->FindUI("Inspector");
			bool InspectorActive = pInspector->IsActive();
			if (ImGui::MenuItem("Inspector", nullptr, &InspectorActive))
			{
				pInspector->SetActive(InspectorActive);
			}

			Ptr<EditorUI> pOutliner = EditorManager::GetInstance()->FindUI("Outliner");
			bool OutlinerActive = pOutliner->IsActive();
			if (ImGui::MenuItem("Outliner", nullptr, &OutlinerActive))
			{
				pOutliner->SetActive(OutlinerActive);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Menu::Tick_UI()
{
}
