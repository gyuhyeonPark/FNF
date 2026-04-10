#include "pch.h"
#include "CStageLoader.h"
#include "SongManager.h"
#include "AssetManager.h"

CStageLoader::CStageLoader(STAGENUM nextStageNum, wstring nextSceneName)
	: CScript(-1), m_nextStageNum(nextStageNum), m_nextSceneName(nextSceneName), m_elapsed(0.f)
{
}

CStageLoader::~CStageLoader()
{
}

void CStageLoader::Begin()
{

}

void CStageLoader::Tick()
{
	m_elapsed += DT;
	
	if (m_elapsed >= 1.f && !m_isCreated)
	{
		CreateScene(m_nextStageNum, DIFF, m_nextSceneName);
		m_isCreated = true;
		m_elapsed = 0.f;
	}

	if (AssetManager::GetInstance()->FindAsset(ASSET_TYPE::SCENE, m_nextSceneName) != nullptr)
	{
		if (m_elapsed >= 2.f)
		{
			ChangeScene(m_nextSceneName);
			ChangeSceneState(SCENE_STATE::PLAY);
		}
	}
}
