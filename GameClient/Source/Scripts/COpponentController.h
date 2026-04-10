#pragma once
#include "CCharacterController.h"

class COpponentController : public CCharacterController
{
	using Super = CCharacterController;

public:
	COpponentController();
	COpponentController(const COpponentController& _Origin);
	virtual ~COpponentController();

	virtual void Begin() override;

public:
	virtual void PlayAnimation(ARROW_DIR dir, bool isFailed, bool autoIdle = true) override;

	CLONE(COpponentController);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;
};

