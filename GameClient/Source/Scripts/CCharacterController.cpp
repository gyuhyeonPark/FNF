#include "pch.h"
#include "CCharacterController.h"
#include "GameObject.h"
#include "CAtlasAnimator.h"
#include "SongManager.h"

CCharacterController::CCharacterController(SCRIPT_TYPE _type)
	: CScript(_type), m_anim(nullptr), m_isMusicStart(false),
	m_idleAnimKey(""), m_elapsed(0.f),
	m_animKeyVector{}
{
}

CCharacterController::CCharacterController(const CCharacterController& _Origin)
	: CScript(_Origin), m_anim(nullptr), m_isMusicStart(false),
	m_idleAnimKey(_Origin.m_idleAnimKey), m_elapsed(0.f),
	m_animKeyVector(_Origin.m_animKeyVector)
{
}

CCharacterController::~CCharacterController()
{
}

void CCharacterController::Tick()
{
	// 실패 시 : m_isAnimStart를 바로 true로 전환, 정해진 시간 이후로 Idle 전환.
	// 성공 시 : release 후에 m_isAnimStart를 true로 전환하기.
	if (m_isAnimStart)
	{
		m_elapsed += DT;

		if (m_elapsed >= (60.f / BPM) * 2)
		{
			m_isAnimStart = false;
			m_elapsed = 0.f;
			m_anim->Play(m_idleAnimKey, true);
		}
	}
}

//void CCharacterController::PlayAnimation(ARROW_DIR dir, bool isFailed)
//{
//	m_anim->Play(m_animKeyVector[(UINT)dir], false);
//	m_elapsed = 0.f;
//	m_isAnimStart = true;
//}
