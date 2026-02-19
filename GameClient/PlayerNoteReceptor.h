#pragma once
#include "NoteReceptor.h"

enum class EFFECT_ANIMKEY
{
	START,
	PRESSED,
	RELEASED,
	END,
};

const string sharedEffectName[3]
{
	"holdCoverStart",
	"holdCover",
	"holdCoverEnd",
};

enum class PKeyState
{
	TAP,
	PRESSED,
	RELEASED,
	END,
};

class PlayerNoteReceptor : public NoteReceptor
{
public:
	PlayerNoteReceptor();
	virtual ~PlayerNoteReceptor();

public:
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void ActivateTapEvent() override;
	virtual void ActivatePressedEvent() override;
	virtual void ActivateReleasedEvent() override;

private:
	void Shoot();

private:
	Ptr<CAtlasAnimator> m_effectAnim[(UINT)PKeyState::END];
	string m_effectNames[3];
	Vec3 m_orginScale;

	bool m_isPressed;
	float m_currentPressStartTiming;
	float m_currentPressEdTiming;
};

