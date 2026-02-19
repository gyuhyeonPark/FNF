#include "pch.h"
#include "CharacterController.h"
#include "GameObject.h"
#include "CAtlasAnimator.h"

CharacterController::CharacterController(Ptr<CAtlasAnimator> anim)
	: m_anim(anim), m_isMusicStart(false)
{
}

CharacterController::~CharacterController()
{
}

void CharacterController::Tick()
{
	if (m_isAnimStart)
	{
		m_elapsed += DT;

		if (m_elapsed >= (60.f / BOPEEBO_BPM) * 2)
		{
			m_isAnimStart = false;
			m_elapsed = 0.f;
			m_anim->Play(m_idleAnimKey, true);
		}
	}
}

//void CharacterController::PlayAnimation(ARROW_DIR dir, bool isFailed)
//{
//	m_anim->Play(m_animKeyVector[(UINT)dir], false);
//	m_elapsed = 0.f;
//	m_isAnimStart = true;
//}
