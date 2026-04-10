#pragma once
#include "CCharacterController.h"

class CAtlasAnimator;

class CPlayerController : public CCharacterController
{
	using Super = CCharacterController;
public:
	CPlayerController();
	//CPlayerController(const CPlayerController& _Origin);
	virtual ~CPlayerController();

public:
	virtual void Begin() override;
	virtual void PlayAnimation(ARROW_DIR dir, bool isFailed = false, bool autoIdle = true) override;

	void PlayHEY();

	CLONE(CPlayerController);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	const UINT m_animOffset;		// miss 및 hey 애니메이션 판별 위한 offset
};

