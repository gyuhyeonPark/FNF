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

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_prev;
	LARGE_INTEGER m_current;

	float m_deltaTime;
	float m_time;
};

