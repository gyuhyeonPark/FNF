#pragma once
/// <summary>
/// 프로그램 관리자 자료형
/// </summary>
class Engine : public SingletonClass<Engine>
{
	SINGLE(Engine);
public:
	~Engine();

private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	POINT		m_Resolution;

public:
	HRESULT Init(HINSTANCE _hInst, UINT _width, UINT _height);
	int Run();

	HWND GetMainWndHwnd() const { return m_hWnd; }
};


