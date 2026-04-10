#include "pch.h"
#include "CollisionManager.h"
#include "SceneManager.h"

SceneManager::SceneManager()
	: m_SceneState(SCENE_STATE::STOP)
{

}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{

}

void SceneManager::Progress()
{
	// 실횅할 씬이 없으면 리턴
	if (m_currentScene == nullptr)
		return;

	// 이전에 등록된 모든 오브젝트들 제거
	m_currentScene->Deregister();

	if (m_SceneState == SCENE_STATE::PLAY)
		m_currentScene->Tick();			// ex) Set Transform

	m_currentScene->FinalTick();	// ex) multiply World Matrix 

	// 충돌 검사 진행
	if (m_SceneState == SCENE_STATE::PLAY)
		CollisionManager::GetInstance()->Progress(m_currentScene);
}

Ptr<GameObject> SceneManager::FindObjectByName(const wstring& _name)
{
	return m_currentScene->FindObjectByName(_name);
}

void SceneManager::ChangeSceneState(SCENE_STATE _nextState)
{
	if (m_SceneState == _nextState)
		return;

	// Stop -> Play
	if (m_SceneState == SCENE_STATE::STOP && _nextState == SCENE_STATE::PLAY)
	{
		// 원본 에셋 레벨의 복제본 레벨을 만들어서 현재 레벨로 가리킨다.
		m_currentScene = m_SharedScene->Clone();
		m_currentScene->SetChanged();
		m_currentScene->Begin();
	}
	else if ((m_SceneState == SCENE_STATE::PLAY || m_SceneState == SCENE_STATE::PAUSE)
		&& _nextState == SCENE_STATE::STOP)
	{
		m_currentScene = m_SharedScene;
		m_currentScene->SetChanged();
		m_currentScene->Begin();
	}

	m_SceneState = _nextState;
}

void SceneManager::ChangeScene(Ptr<AScene> _nextScene)
{
	m_currentScene = m_SharedScene = _nextScene;

	m_SceneState = SCENE_STATE::STOP;
}
