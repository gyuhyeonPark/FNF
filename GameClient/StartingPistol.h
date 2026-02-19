#pragma once
#include "CScript.h"

class GameManager;
class EventManager;

/// <summary>
/// 모든 맵의 시작을 알리는 클래스.
/// Ready-Set-Go 텍스처 출력 후 끝나는 타이밍에 노래가 시작하도록 해주기
/// </summary>
class StartingPistol : public CScript
{
public:
	StartingPistol();
	virtual ~StartingPistol();

public:
	virtual void Begin() override;
	virtual void Tick() override;

	void SetBPM(float bpm) { m_duration = 60.f / bpm; }
	void SetGM(Ptr<GameManager> gm) { m_gm = gm; }
	void SetEM(Ptr<EventManager> em) { m_em = em; }

private:
	float m_elapsed;
	float m_duration;		// 4분음표 한 박

	int m_mtrlTexIdx;

	std::function<void()> m_funcArr[5];
	int m_funcIdx;

	Ptr<GameManager> m_gm;
	Ptr<EventManager> m_em;
};

// texture의 shader 수치 조정은 material constantbuffer를 사용하도록 한다.

