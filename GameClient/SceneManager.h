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

private:
	Ptr<AScene> m_currentScene;
};

