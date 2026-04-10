#include "pch.h"
#include "KeyManager.h"

UINT g_KeyIndex[(UINT)KEY::KEY_END] =
{
	'W',
	'S',
	'A',
	'D',

	'Q',
	'E',
	
	'R',
	'F',
	'X',

	VK_LBUTTON,
	VK_RBUTTON,
	VK_MBUTTON,

	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	VK_RETURN,
	VK_MENU,
	VK_CONTROL,

	VK_LSHIFT,
	VK_RSHIFT,
	VK_SPACE,
	VK_HOME,
	VK_ESCAPE,

	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
};

KeyManager::KeyManager()
	: m_wheel(0), m_wheelChanged(false), m_active(true)
{
}

KeyManager::~KeyManager()
{
}

void KeyManager::Init()
{
	m_vecKeys.resize(UINT(KEY::KEY_END));
}

void KeyManager::Tick()
{
	if (GetFocus() == Engine::GetInstance()->GetMainWndHwnd() && m_active)
	{
		for (UINT i = 0; i < (UINT)KEY::KEY_END; ++i)
		{
			if (GetAsyncKeyState(g_KeyIndex[i]) & 0x8000)
			{
				// 이전에도 눌려있었다.
				if (m_vecKeys[i].Pressed)
				{
					m_vecKeys[i].state = PRESSED;
				}
				// 지금은 눌려있지만 이전엔 눌리지 않았다.
				else
				{
					m_vecKeys[i].state = TAP;
				}

				m_vecKeys[i].Pressed = true;
			}
			else
			{
				// 이전에도 눌려있었다.
				if (m_vecKeys[i].Pressed)
				{
					m_vecKeys[i].state = RELEASED;
				}
				else
				{
					m_vecKeys[i].state = NONE;
				}

				m_vecKeys[i].Pressed = false;
			}
		}
	}
	else
	{
		for (UINT i = 0; i < (UINT)KEY::KEY_END; ++i)
		{
			if (m_vecKeys[i].state == KEY_STATE::TAP
				|| m_vecKeys[i].state == KEY_STATE::PRESSED)
				m_vecKeys[i].state = KEY_STATE::RELEASED;
			else
				m_vecKeys[i].state = KEY_STATE::NONE;

			m_vecKeys[i].Pressed = false;
		}
	}

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(Engine::GetInstance()->GetMainWndHwnd(), &pt);

	m_mousePrevPos = m_mousePos;
	m_mousePos = Vec2((float)pt.x, (float)pt.y);

	m_mouseDir = m_mousePos - m_mousePrevPos;

	if (m_wheelChanged)
		m_wheelChanged = false;
	else
		m_wheel = 0;
}
