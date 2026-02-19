#include "pch.h"
#include "CPlayerController.h"
#include "CAtlasAnimator.h"
#include "AssetManager.h"
#include "CImage.h"

CPlayerController::CPlayerController(Ptr<CAtlasAnimator> anim)
	: Super(anim), m_animOffset(UINT(ARROW_DIR::END))
{
	m_animKeyVector.resize(UINT(ARROW_DIR::END) + UINT(ARROW_DIR::END) + 1);		// dir + missdir + hey
	m_animKeyVector[UINT(ARROW_DIR::LEFT)] = "BF NOTE LEFT";
	m_animKeyVector[UINT(ARROW_DIR::DOWN)] = "BF NOTE DOWN";
	m_animKeyVector[UINT(ARROW_DIR::UP)] = "BF NOTE UP";
	m_animKeyVector[UINT(ARROW_DIR::RIGHT)] = "BF NOTE RIGHT";
	
	// Miss Anim
	m_animKeyVector[UINT(ARROW_DIR::LEFT) + m_animOffset] = "BF NOTE LEFT MISS";
	m_animKeyVector[UINT(ARROW_DIR::DOWN) + m_animOffset] = "BF NOTE DOWN MISS";
	m_animKeyVector[UINT(ARROW_DIR::UP) + m_animOffset] = "BF NOTE UP MISS";
	m_animKeyVector[UINT(ARROW_DIR::RIGHT) + m_animOffset] = "BF NOTE RIGHT MISS";

	m_animKeyVector[UINT(ARROW_DIR::END) + m_animOffset] = "BF HEY!!";

	m_idleAnimKey = "BF idle dance";
}

CPlayerController::~CPlayerController()
{
}

void CPlayerController::Begin()
{
	m_anim->Play(m_idleAnimKey, true);
}

void CPlayerController::PlayAnimation(ARROW_DIR dir, bool isFailed)
{
	int idx = 0;
	if (isFailed)
	{
		idx = (UINT)dir + m_animOffset;
		SOUNDMANAGER->PlayMiss();
	}
	else
		idx = (UINT)dir;

	m_anim->Play(m_animKeyVector[idx], false);
	m_isAnimStart = true;
	m_elapsed = 0.f;
}

void CPlayerController::PlayHEY()
{
	m_anim->Play(m_animKeyVector[UINT(ARROW_DIR::END) + m_animOffset], false);
	m_isAnimStart = true;
	m_elapsed = 0.f;
}

