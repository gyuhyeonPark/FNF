#pragma once
#include "AScene.h"

class SceneManager : public SingletonClass<SceneManager>
{
	SINGLE(SceneManager);
public:
	~SceneManager();

public:
	void Init();
	void Progress();

	Ptr<AScene> GetCurrentScene() const { return m_currentScene; }
	Ptr<GameObject> FindObjectByName(const wstring& _name);
	SCENE_STATE GetSceneState() { return m_SceneState; }

private:
	void ChangeScene(Ptr<AScene> _nextScene);
	void ChangeSceneState(SCENE_STATE _nextState);

private:
	Ptr<AScene> m_currentScene;
	Ptr<AScene> m_SharedScene;
	SCENE_STATE m_SceneState;

	friend class TaskManager;
};

