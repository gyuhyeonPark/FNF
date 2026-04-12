#include "pch.h"
#include "Engine.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "RenderManager.h"
#include "EditorManager.h"
#include "SongManager.h"

LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT Engine::Init(HINSTANCE _hInst, UINT _width, UINT _height, bool _EditorMode)
{
    m_hInst = _hInst;
    m_Resolution.x = _width;
    m_Resolution.y = _height;
    m_EditorMode = _EditorMode;

    // 생성시킬 윈도우(창) 옵션 설정
    WNDCLASSEXW wcex = {};

    wcex.lpszClassName = L"MyGame";
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;             // 함수 포인터 등록
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_GAMECLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_GAMECLIENT);
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    if (!RegisterClassExW(&wcex))
    {
        return E_FAIL;
    }

    // 등록한 윈도우 설정 데이터를 기반으로, 윈도우(창) 하나 생성

    UINT Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    m_hWnd = CreateWindowW(L"MyGame", L"MyGame", Style, CW_USEDEFAULT, 0, CW_USEDEFAULT
        , 0, nullptr, nullptr, m_hInst, nullptr);

    // 윈도우 생성 실패시 프로그램 종료
    if (!m_hWnd)
    {
        return E_FAIL;
    }

    // 생성한 윈도우를 화면에 보이도록 설정
    ShowWindow(m_hWnd, true);
    UpdateWindow(m_hWnd);

    RECT rt = {0, 0, GWinSizeX, GWinSizeY};
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !GetMenu(m_hWnd));

    // 윈도우 크기 및 위치 변경      
    SetWindowPos(m_hWnd, nullptr, 0, 0, m_Resolution.x, m_Resolution.y, 0);

    // Graphics Initialization
    Graphics::GetInstance()->Init(m_hWnd);

    TimeManager::GetInstance()->Init();
    KeyManager::GetInstance()->Init();
    SOUNDMANAGER->Init();
    AssetManager::GetInstance()->Init();

    SceneManager::GetInstance()->Init();

    RenderManager::GetInstance()->Init();

    if (m_EditorMode)
    {
        EditorManager::GetInstance()->Init(m_hWnd);
    }


    {
        CreateMainMenuScene();
        ChangeScene(L"MainMenuScene");
        ChangeSceneState(SCENE_STATE::PLAY);
    }


    SongManager::GetInstance()->CustomON();

    
/*    {
        CreateSongEditorScene();
        ChangeScene(L"SongEditor");
        ChangeSceneState(SCENE_STATE::PLAY);
    }*/
    return S_OK;
}