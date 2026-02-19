#pragma once
#include "CharacterController.h"

class CAtlasAnimator;

class CPlayerController : public CharacterController
{
	using Super = CharacterController;
public:
	CPlayerController(Ptr<CAtlasAnimator> anim);
	virtual ~CPlayerController();

public:
	virtual void Begin() override;
	virtual void PlayAnimation(ARROW_DIR dir, bool isFailed = false) override;

	void PlayHEY();

private:
	const UINT m_animOffset;		// miss 및 hey 애니메이션 판별 위한 offset
};

