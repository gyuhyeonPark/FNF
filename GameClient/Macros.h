#pragma once


#define SINGLE(classname) private:\
							classname();\
							classname(const classname&) = delete;\
							friend class SingletonClass<classname>;


#define CHECK(hr) assert(hr == S_OK)

#define DEVICE	Graphics::GetInstance()->GetDevice()
#define DEVICE_CONTEXT		Graphics::GetInstance()->GetDeviceContext()
#define RTV		Graphics::GetInstance()->GetRTV()
#define DSV		Graphics::GetInstance()->GetDSV()
#define PARSER	Parser::GetInstance()
#define SOUNDMANAGER	SoundManager::GetInstance()
#define DT		TimeManager::GetInstance()->GetDeltaTime()

#define KEYCHECK(key, state)		state == KeyManager::GetInstance()->GetKeyState(key)
#define KEY_NONE(key)				KEYCHECK(key, KEY_STATE::NONE)
#define KEY_TAP(key)				KEYCHECK(key, KEY_STATE::TAP)
#define KEY_PRESSED(key)			KEYCHECK(key, KEY_STATE::PRESSED)
#define KEY_RELEASED(key)			KEYCHECK(key, KEY_STATE::RELEASED)

#define GET(Type, MemName) Type Get##MemName() {return m_##MemName;}
#define SET(Type, MemName) void Set##MemName(Type _Data) {m_##MemName = _Data;}
#define GET_SET(Type, MemName) GET(Type, MemName) SET(Type, MemName)

#define MAX_LAYER 32
#define NOTE_SIZE 8
#define IDLE_FRAME_SIZE 14

#define SICK 50
#define GOOD 70
#define BAD 90

#define BOPEEBO_BPM 100
