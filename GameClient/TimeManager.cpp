#include "pch.h"
#include "TimeManager.h"
#include "SceneManager.h"

TimeManager::TimeManager()
	:	m_deltaTime(0.f), m_time(0.f), m_frequency(), m_prev(), m_current(), m_FPS(0)
{

}

TimeManager::~TimeManager()
{
}

void TimeManager::Init()
{
	// 1 초동안 가능한 카운팅 횟수
	QueryPerformanceFrequency(&m_frequency);

	// 초기 카운트
	QueryPerformanceCounter(&m_current);
	QueryPerformanceCounter(&m_prev);
}

void TimeManager::Tick()
{
	++m_FPS;

	// 현재 카운팅 가져오기
	QueryPerformanceCounter(&m_current);

	// 이전과 현재 카운팅 차이를 Frequency 로 나눠서 1 프레임동안 진행한 시간값을 구하기
	m_deltaTime = (float)(m_current.QuadPart - m_prev.QuadPart) / (float)m_frequency.QuadPart;

	const float targetFrame = 1.f / 120.f;

	if (m_deltaTime < targetFrame)
	{
		Sleep((DWORD)((targetFrame - m_deltaTime) * 1000.f));
	}

	// Prev 카운팅을 다시 현재카운팅으로 맞추기
	m_prev = m_current;

	// 누적 시간 계산
	m_time += m_deltaTime;

	// 1 초마다 if 수행
	if (1.f < m_time)
	{
		wchar_t buff[255] = {};
		swprintf_s(buff, 255, L"DeltaTime : %f, FPS : %d", m_deltaTime, m_FPS);
		SetWindowText(Engine::GetInstance()->GetMainWndHwnd(), buff);

		m_FPS = 0;
		m_time -= 1.f;
	}

	// Game Engine용 Time
	g_Global.EngineDT = m_deltaTime;
	g_Global.EngineTime += m_deltaTime;

	// Level 이 Pause 나 Stop 상태라면
	if (SCENE_STATE::PLAY != SceneManager::GetInstance()->GetSceneState())
	{
		g_Global.DeltaTime = m_deltaTime = 0.f;
		g_Global.Time = 0.f;
	}
	// Level 이 Play 상태
	else
	{
		// Game Content 용 Time
		g_Global.DeltaTime = m_deltaTime;
		g_Global.Time += m_deltaTime;
	}
}
