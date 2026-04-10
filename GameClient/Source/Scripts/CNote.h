#pragma once
#include "CScript.h"

class CAtlasAnimator;

const char* noteSpriteName[4] =
{
	"noteLeft",
	"noteDown",
	"noteUp",
	"noteRight",
};

class CNote : public CScript
{
public:
	CNote();
	CNote(const CNote& _Origin);
	virtual ~CNote();

public:
	virtual void Begin() override;	
	virtual void Tick() override;

	void SetNoteSpeed(float speed) { m_speed = speed; }
	void SetArrowDir(ARROW_DIR dir) { m_dir = dir; }
	CLONE(CNote);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

	void IsBody(bool _boolen) { m_isBody = _boolen; }
	void ActivateVanish();

private:
	float m_speed;
	ARROW_DIR m_dir;

	friend class CNoteReceptor;

	bool m_isBody;
};

