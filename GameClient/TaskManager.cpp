#include "pch.h"
#include "TaskManager.h"
#include "SceneManager.h"
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
			}
		}
		break;
		}
	}

	m_vecTask.clear();
}
