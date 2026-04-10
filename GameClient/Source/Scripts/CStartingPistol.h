#pragma once
#include "CScript.h"

class CGameManager;
class CEventManager;

/// <summary>
/// 모든 맵의 시작을 알리는 클래스.
/// Ready-Set-Go 텍스처 출력 후 끝나는 타이밍에 노래가 시작하도록 해주기
/// </summary>
class CStartingPistol : public CScript
{
public:
	CStartingPistol();
	CStartingPistol(const CStartingPistol& _Origin);
	virtual ~CStartingPistol();

public:
	virtual void Begin() override;
	virtual void Tick() override;

	void SetBPM(float bpm) { m_duration = 60.f / bpm; }
	void SetGM(Ptr<CGameManager> gm) { m_gm = gm; }
	void SetEM(Ptr<CEventManager> em) { m_em = em; }

	CLONE(CStartingPistol);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	float m_elapsed;
	float m_duration;		// 4분음표 한 박

	int m_mtrlTexIdx;

	std::function<void()> m_funcArr[5];
	int m_funcIdx;

	Ptr<CGameManager> m_gm;
	Ptr<CEventManager> m_em;
};

// texture의 shader 수치 조정은 material constantbuffer를 사용하도록 한다.

