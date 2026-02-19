#include "pch.h"
#include "COpponentController.h"
#include "CAtlasAnimator.h"

COpponentController::COpponentController(Ptr<CAtlasAnimator> anim)
	: Super(anim)
{
	m_animKeyVector.resize(UINT(ARROW_DIR::END));
	m_animKeyVector[UINT(ARROW_DIR::LEFT)] = "singLEFT";
	m_animKeyVector[UINT(ARROW_DIR::DOWN)] = "singDOWN";
	m_animKeyVector[UINT(ARROW_DIR::UP)] = "singUP";
	m_animKeyVector[UINT(ARROW_DIR::RIGHT)] = "singRIGHT";

	m_idleAnimKey = "idle";
}

COpponentController::~COpponentController()
{
}

void COpponentController::Begin()
{
	m_anim->Play(m_idleAnimKey, true);
}

void COpponentController::PlayAnimation(ARROW_DIR dir, bool isFailed)
{
	m_anim->Play(m_animKeyVector[(UINT)dir], false);
	m_isAnimStart = true;
	m_elapsed = 0.f;
}
