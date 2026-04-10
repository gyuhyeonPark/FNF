#pragma once
class TimeManager : public SingletonClass<TimeManager>
{
	SINGLE(TimeManager);

public:
	~TimeManager();

public:
	void Init();
	void Tick();

	float GetDeltaTime() { return m_deltaTime; }
	float GetTime() { return g_Global.Time; }

	float GetEngineDT() { return g_Global.EngineDT; }
	float GetEngineTime() { return g_Global.EngineTime; }

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_prev;
	LARGE_INTEGER m_current;

	UINT  m_FPS;

	float m_deltaTime;
	float m_time;
};

