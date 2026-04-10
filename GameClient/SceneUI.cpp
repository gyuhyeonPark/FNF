#include "pch.h"
#include "SceneUI.h"

SceneUI::SceneUI()
	: AssetUI(ASSET_TYPE::SCENE)
{
}

SceneUI::~SceneUI()
{
}

void SceneUI::Tick_UI()
{
	OutputTitle();
}