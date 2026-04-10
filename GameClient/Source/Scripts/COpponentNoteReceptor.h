#pragma once
#include "CNoteReceptor.h"

class COpponentNoteReceptor : public CNoteReceptor
{
public:
	COpponentNoteReceptor();
	virtual ~COpponentNoteReceptor();

public:
	virtual void Begin() override;
	virtual void Tick() override;

	CLONE(COpponentNoteReceptor);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	virtual void ActivateTapEvent() override;
	virtual void ActivatePressedEvent() override;
	virtual void ActivateReleasedEvent() override;

private:
	float m_idleAnimDuration;
	float m_idleAnimElapsed;
	bool m_isIdleTimerOn;

	Ptr<CAtlasAnimator> m_effectAnim;
	string m_effectName;
};

