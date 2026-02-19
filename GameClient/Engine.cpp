#include "pch.h"
#include "Engine.h"
#include "SceneManager.h"
#include "RenderManager.h"
#include "TaskManager.h"
#include "EditorManager.h"

Engine::Engine()
    : m_hInst(nullptr), m_hWnd(nullptr), m_Resolution{}
{

}

Engine::~Engine()
{
}

int Engine::Run()
{
    TimeManager::GetInstance()->Tick();
    KeyManager::GetInstance()->Tick();
    SOUNDMANAGER->Update();

    // SceneManager 업데이트
    SceneManager::GetInstance()->Progress();

    RenderManager::GetInstance()->Progress();

    EditorManager::GetInstance()->Progress();       // 게임 오브젝트 렌더링 후 -> Imgui 렌더링 -> present

    // 렌더타겟에 그려진 그림을, 윈도우 비트맵으로 복사
    Graphics::GetInstance()->Present();


    TaskManager::GetInstance()->Progress();

    return S_OK;
}
