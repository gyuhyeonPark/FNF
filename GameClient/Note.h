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

class Note : public CScript
{
public:
	Note(float speed, ARROW_DIR dir);
	virtual ~Note();

public:
	virtual void Begin() override;	
	virtual void Tick() override;

private:
	float m_speed;
	ARROW_DIR m_dir;
	Ptr<CAtlasAnimator> m_anim;
	friend class NoteReceptor;
};

