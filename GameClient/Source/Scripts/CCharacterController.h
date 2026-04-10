#pragma once
#include "CScript.h"
#include "CNoteReceptor.h"

class CCharacterController : public CScript
{
public:
	CCharacterController(SCRIPT_TYPE _type);
	CCharacterController(const CCharacterController& _Origin);
	virtual ~CCharacterController();

public:
	virtual void Tick();

	// 기본적인 애니메이션 재생 함수, Player는 상이
	virtual void PlayAnimation(ARROW_DIR dir, bool isFailed = false, bool autoIdle = true) = 0;

	virtual void Play() { m_isMusicStart = true; }

	virtual	void IdleTimerOn() { m_isAnimStart = true; }

	virtual CScript* Clone() = 0;

public:
	void SetAnimator(Ptr<CAtlasAnimator> anim) { m_anim = anim; }

protected:
	Ptr<CAtlasAnimator> m_anim;
	vector<string> m_animKeyVector;
	string m_idleAnimKey;

	bool m_isMusicStart;

	// 특정 애니메이션 재생 후 몇 초 후에 다시 Idle 애니메이션 재생하기
	float m_elapsed;
	bool m_isAnimStart;
};

