#include "pch.h"
#include "CStageTransition.h"
#include "SceneManager.h"
#include "SongManager.h"

CStageTransition::CStageTransition(float duration)
	: CScript(-1), m_trigger(false), m_elapsed(0.f), m_duration(duration), m_isSceneCreated(false)
{
}

CStageTransition::CStageTransition(const CStageTransition& origin)
	: CScript(origin), m_trigger(false), m_elapsed(0.f), m_duration(origin.m_duration), m_isSceneCreated(false)
{
}

CStageTransition::~CStageTransition()
{
}

void CStageTransition::Tick()
{
	if (stageIdx == 4)
		return;

	if (m_trigger)
	{
		if (!m_isSceneCreated)
		{
			m_isSceneCreated = true;
			CreateLoadingScene(stageArr[stageIdx], stageNameArr[stageIdx]);
		}

		m_elapsed += DT;

		if (m_elapsed >= m_duration)
		{
			m_elapsed = 0.f;

			ChangeScene(L"Loading_" + stageNameArr[stageIdx]);
			ChangeSceneState(SCENE_STATE::PLAY);
			SongManager::GetInstance()->ChangeSong(stageArr[stageIdx]);
			++stageIdx;

			SOUNDMANAGER->MuteSong(true);
		}
	}
}
