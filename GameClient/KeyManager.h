#pragma once

enum class KEY
{
	W,
	S,
	A,
	D,

	Q,
	E,

	R,
	F,
	X,

	LBTN,
	RBTN,
	MBTN,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	ENTER,
	ALT,
	CTRL,

	LSHIFT,
	RSHIFT,
	SPACE, 
	HOME,
	ESC,
	
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,


	KEY_END
};

enum KEY_STATE
{
	NONE,
	TAP,
	PRESSED,
	RELEASED,
};

struct KeyInfo
{
	KEY_STATE state;	// 키의 현재 상태
	bool Pressed;		// 이전에 눌린적이 있는가
};

class KeyManager : public SingletonClass<KeyManager>
{
	SINGLE(KeyManager);
public:
	~KeyManager();

public:
	void Init();
	void Tick();

	KEY_STATE GetKeyState(KEY _key)
	{
		return m_vecKeys[(UINT)_key].state;
	}

	GET(Vec2, mouseDir);

	int GetMouseWheel() { return m_wheel; }
	void SetMouseWheel(int _Wheel)
	{
		m_wheelChanged = true;
		m_wheel = _Wheel / abs(_Wheel);
	}

	bool IsActive() { return m_active; }
	void SetActive(bool _boolen) { m_active = _boolen; }

private:
	vector<KeyInfo> m_vecKeys;

	Vec2 m_mousePos;
	Vec2 m_mousePrevPos;
	Vec2 m_mouseDir;

	bool m_wheelChanged;
	int m_wheel;  // 1, 0, -1

	bool m_active;
};

