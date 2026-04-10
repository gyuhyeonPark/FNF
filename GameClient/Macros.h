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
#define TIME	TimeManager::GetInstance()->GetTime()
#define E_DT	TimeManager::GetInstance()->GetEngineDT()
#define E_Time	TimeManager::GetInstance()->GetEngineTime()

#define CONTENT_PATH PathManager::GetInstance()->GetContentsPath()

#define KEYCHECK(key, state)		state == KeyManager::GetInstance()->GetKeyState(key)
#define KEY_NONE(key)				KEYCHECK(key, KEY_STATE::NONE)
#define KEY_TAP(key)				KEYCHECK(key, KEY_STATE::TAP)
#define KEY_PRESSED(key)			KEYCHECK(key, KEY_STATE::PRESSED)
#define KEY_RELEASED(key)			KEYCHECK(key, KEY_STATE::RELEASED)

#define GET(Type, MemName) Type Get##MemName() {return m_##MemName;}
#define SET(Type, MemName) void Set##MemName(Type _Data) {m_##MemName = _Data;}
#define GET_SET(Type, MemName) GET(Type, MemName) SET(Type, MemName)
#define CLONE(Type) Type* Clone() { return new Type(*this); }


#define MAX_LAYER 32
#define NOTE_SIZE 8
#define IDLE_FRAME_SIZE 14

#define SICK 100
#define GOOD 130
#define BAD 150

#define MAINMENU_BPM 102
#define BOPEEBO_BPM 100
#define FRESH_BPM 120
#define DADBATTLE_BPM 180
#define SENPAI_BPM 144
#define SQUARE_BPM 99
#define WHIPLASH_BPM 216
#define LTR_BPM 164

#define MAGIC_NUM 0.05f		// 노트를 조금 늦게 도착하게 해주는 수

