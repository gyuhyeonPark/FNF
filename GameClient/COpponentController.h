#pragma once
#include "CharacterController.h"
class COpponentController : public CharacterController
{
	using Super = CharacterController;

public:
	COpponentController(Ptr<CAtlasAnimator> anim);
	virtual ~COpponentController();

	virtual void Begin() override;

public:
	virtual void PlayAnimation(ARROW_DIR dir, bool isFailed) override;
};

