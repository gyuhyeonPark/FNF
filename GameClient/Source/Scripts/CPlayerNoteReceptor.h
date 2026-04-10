#pragma once
#include "CNoteReceptor.h"

enum class EFFECT_ANIMKEY
{
	START1,
	START2,
	PRESSED,
	RELEASED,
	END,
};

const string sharedEffectName[4]
{
	"note impact 1 ",
	"note impact 2 ",
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

class CPlayerNoteReceptor : public CNoteReceptor
{
public:
	CPlayerNoteReceptor();
	CPlayerNoteReceptor(const CPlayerNoteReceptor& _Orgin);
	virtual ~CPlayerNoteReceptor();

public:
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void ActivateTapEvent() override;
	virtual void ActivatePressedEvent() override;
	virtual void ActivateReleasedEvent() override;

	CLONE(CPlayerNoteReceptor);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

	bool IsWellPressed() { return m_isWellPressed; }

private:
	void Shoot();

private:
	Ptr<CAtlasAnimator> m_effectAnim[(UINT)PKeyState::END];
	string m_effectNames[4];
	Vec3 m_orginScale;

	bool m_isPressed;
	float m_currentPressStartTiming;
	float m_currentPressEdTiming;

	bool m_isWellPressed;
};

