#include "pch.h"
#include "TaskManager.h"
#include "SceneManager.h"
#include "AssetManager.h"
#include "GameObject.h"

TaskManager::TaskManager()
{
}

TaskManager::~TaskManager()
{
}

void TaskManager::Progress()
{
	m_garbage.clear();

	for (int i = 0; i < m_vecTask.size(); i++)
	{
		switch (m_vecTask[i].type)
		{
			case TASK_TYPE::CREATE_OBJECT:
			{
				Ptr<GameObject> newObj = (GameObject*)m_vecTask[i].param_0;
				Ptr<AScene> curScene = SceneManager::GetInstance()->GetCurrentScene();
				curScene->AddObject(m_vecTask[i].param_1, newObj);
				curScene->SetChanged();

				// 씬이 Play 상태일 때 추가된 오브젝트는, 씬 시작 시점 때 Begin을 호출받지 못했기 때문에,
				// 씬에 스폰될 때 Begin을 호출하게 한다.
				if (SceneManager::GetInstance()->GetSceneState() == SCENE_STATE::PLAY)
					newObj->Begin();
			}
			break;

			case TASK_TYPE::DESTROY_OBJECT:
			{
				Ptr<GameObject> pObj = (GameObject*)m_vecTask[i].param_0;

				if (!pObj->IsDead())
				{
					pObj->m_dead = true;
					m_garbage.push_back(pObj);
					Ptr<AScene> pCurScene = SceneManager::GetInstance()->GetCurrentScene();
					pCurScene->SetChanged();
				}
			}
			break;
		
			case TASK_TYPE::CHANGE_SCENE:
			{
				const wchar_t* pSceneName = (const wchar_t*)m_vecTask[i].param_0;
				Ptr<AScene> pScene = AssetManager::GetInstance()->Find<AScene>(pSceneName);
				SceneManager::GetInstance()->ChangeScene(pScene);
			}
			break;
		
			case TASK_TYPE::CHANGE_SCENE_STATE:
			{
				SCENE_STATE NextState = (SCENE_STATE)m_vecTask[i].param_0;
				SceneManager::GetInstance()->ChangeSceneState(NextState);
			}
			break;
		}
	}

	m_vecTask.clear();
}
