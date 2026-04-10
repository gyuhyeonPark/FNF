#include "pch.h"
#include "Menu.h"

#include "EditorManager.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "ContentUI.h"

#include "Inspector.h"
#include "GameObject.h"

#include "Source/ScriptMgr.h"

Menu::Menu()
	: EditorUI("Menu")
{
}

Menu::~Menu()
{
}

void Menu::Tick_UI()
{
}

void Menu::Tick()
{
	if (ImGui::BeginMainMenuBar())
	{
		File();

		Scene();

		View();

		GameObjectMenu();

		Asset();

		ImGui::EndMainMenuBar();
	}
}

void Menu::File()
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
}

void Menu::Scene()
{
	if (ImGui::BeginMenu("Scene"))
	{
		bool HasScene = SceneManager::GetInstance()->GetCurrentScene().Get();
		bool IsPlay = false, IsPause = false, IsStop = false;
		if (HasScene)
		{
			SCENE_STATE curState = SceneManager::GetInstance()->GetSceneState();
			if (curState == SCENE_STATE::PLAY)
				IsPlay = true;
			else if (curState == SCENE_STATE::PAUSE)
				IsPause = true;
			else if (curState == SCENE_STATE::STOP)
				IsStop = true;
		}

		if (ImGui::MenuItem("Play", nullptr, nullptr, HasScene && !IsPlay))
		{
			ChangeSceneState(SCENE_STATE::PLAY);
		}

		if (ImGui::MenuItem("Pause", nullptr, nullptr, HasScene && IsPlay))
		{
			ChangeSceneState(SCENE_STATE::PAUSE);
		}

		if (ImGui::MenuItem("Stop", nullptr, nullptr, HasScene && !IsStop))
		{
			ChangeSceneState(SCENE_STATE::STOP);
		}

		ImGui::EndMenu();
	}
}

void Menu::View()
{
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
}

void Menu::GameObjectMenu()
{
	if (ImGui::BeginMenu("GameObject"))
	{

		ImGui::EndMenu();
	}
}

void Menu::Asset()
{
	if (ImGui::BeginMenu("Asset"))
	{
		if (ImGui::BeginMenu("Create Asset"))
		{
			if (ImGui::MenuItem("Create Material"))
			{
				Ptr<AMaterial> pMtrl = new AMaterial;
				wstring Key = GetAssetName(ASSET_TYPE::MATERIAL, L"Material\\Default Material");
				AssetManager::GetInstance()->AddAsset(Key, pMtrl.Get());
			}

			if (ImGui::MenuItem("Create Sprite"))
			{

			}

			if (ImGui::MenuItem("Create Flipbook"))
			{

			}

			if (ImGui::MenuItem("Create TileMap"))
			{

			}
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

wstring Menu::GetAssetName(ASSET_TYPE _Type, const wstring& _Name)
{
	wstring Ext;

	switch (_Type)
	{
	case ASSET_TYPE::MESH:
		Ext = L".mesh";
		break;
	case ASSET_TYPE::MATERIAL:
		Ext = L".mtrl";
		break;
	case ASSET_TYPE::TEXTURE:
	case ASSET_TYPE::SOUND:
	case ASSET_TYPE::GRAPHICSHADER:
	case ASSET_TYPE::COMPUTESHADER:
		assert(nullptr);
		break;
	case ASSET_TYPE::SPRITE:
		Ext = L".sprite";
		break;
	case ASSET_TYPE::FLIPBOOK:
		Ext = L".flip";
		break;
	case ASSET_TYPE::PREFAB:
		Ext = L".pref";
		break;
	case ASSET_TYPE::SCENE:
		Ext = L".scene";
		break;
	}

	int i = 0;
	while (true)
	{
		wchar_t Num[50] = {};
		swprintf_s(Num, 50, L"_%d", i);

		wstring AssetName = wstring(_Name + Num + Ext);
		if (nullptr == AssetManager::GetInstance()->FindAsset(_Type, AssetName))
		{
			return AssetName;
		}

		i++;
	}
}