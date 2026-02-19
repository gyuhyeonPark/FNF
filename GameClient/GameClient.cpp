#include "pch.h"
#include "GameClient.h"
#include "imgui_impl_win32.h"

// 전역 변수:
HINSTANCE   hInst;

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// SAL : 주석 언어
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // 메모리 누수 확인 출력 창에 알려진 일련변호를 아래의 함수에 매개변수에 넣어주자.
    // 어느 부분에서 누수가 됐는지 함수 호출 스택타고 들어가서 확인 가능하게 해주는 함수
    // 문제를 해결했다면 아래 함수를 주석 처리 해주자.
    // _CrtSetBreakAlloc(int);
    
    // Engine 초기화   
    Engine::GetInstance()->Init(hInstance, GWinSizeX, GWinSizeY);
    //if (FAILED(Engine::GetInstance()->Init(hInstance, GWinSizeX, GWinSizeY)))
    //    return 0;
    
    // 메세지 루프
    // 단축키 테이블 정보 핸들 받음
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMECLIENT));

    // 윈도우에 발생한 사건(이벤트, 메세지)를 받을 구조체
    MSG msg = { 0 };

    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            CHECK(Engine::GetInstance()->Run());
        }
    }
    return (int)msg.wParam;
}

// 프로시저 함수
// 윈도우에 발생한 사건(이벤트, 메세지)을 처리해주는 함수
// DispatchMessage에서 해당 함수를 호출
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_MOUSEWHEEL:
    {
        KeyManager::GetInstance()->SetMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
