#include "pch.h"
#include "Note.h"
#include "GameObject.h"
#include "CAtlasAnimator.h"
#include "NoteReceptor.h"

Note::Note(float speed, ARROW_DIR dir)
	: m_speed(speed), m_dir(dir), m_anim(nullptr)
{
}

Note::~Note()
{
}

void Note::Begin()
{
	if (m_anim != nullptr)
		m_anim->Play(noteSpriteName[UINT(m_dir)], false);
}

void Note::Tick()
{
	Vec3 pos = Transform()->GetRelativePosition();

	pos.y += DT * m_speed;

	Transform()->SetRelativePosition(pos);
}
