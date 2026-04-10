#include "pch.h"
#include "COpponentController.h"
#include "CAtlasAnimator.h"

COpponentController::COpponentController()
	: Super(SCRIPT_TYPE::OPPONENTCONTROLLER)
{
	m_animKeyVector.resize(UINT(ARROW_DIR::END));
	m_animKeyVector[UINT(ARROW_DIR::LEFT)] = "singLEFT";
	m_animKeyVector[UINT(ARROW_DIR::DOWN)] = "singDOWN";
	m_animKeyVector[UINT(ARROW_DIR::UP)] = "singUP";
	m_animKeyVector[UINT(ARROW_DIR::RIGHT)] = "singRIGHT";

	m_idleAnimKey = "idle";
}

COpponentController::COpponentController(const COpponentController& _Origin)
	: Super(_Origin)
{
}

COpponentController::~COpponentController()
{
}

void COpponentController::Begin()
{
	m_anim = dynamic_cast<CAtlasAnimator*>(GetOwner()->GetComponent(COMPONENT_TYPE::ATLASANIMATOR).Get());
	m_anim->Play(m_idleAnimKey, true);
}

void COpponentController::PlayAnimation(ARROW_DIR dir, bool isFailed, bool autoIdle)
{
	m_anim->Play(m_animKeyVector[(UINT)dir], false);
	m_isAnimStart = autoIdle;
	m_elapsed = 0.f;
}

void COpponentController::SaveToSceneFile(FILE* _File)
{
}

void COpponentController::LoadFromSceneFile(FILE* _File)
{
}
