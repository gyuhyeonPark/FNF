#include "pch.h"
#include "CNote.h"
#include "GameObject.h"
#include "CAtlasAnimator.h"
#include "CNoteReceptor.h"
#include "TaskManager.h"
#include "AssetManager.h"

CNote::CNote()
	: CScript(SCRIPT_TYPE::NOTE),
	m_speed(500.f), m_dir(ARROW_DIR::END), m_isBody(false)
{
}

CNote::CNote(const CNote& _Origin)
	: CScript(_Origin),
	m_speed(_Origin.m_speed), m_dir(ARROW_DIR::END),
	m_isBody(_Origin.m_isBody)
{
}

CNote::~CNote()
{
}

void CNote::Begin()
{
	if (m_isBody)
	{
		Ptr<AMaterial> noteMtrl = LOAD(AMaterial, L"Material\\NoteBar.mtrl")->Clone();
		noteMtrl->SetScalar(FLOAT_0, GetOwner()->GetParent()->Transform()->GetWorldPos().y);
		noteMtrl->SetScalar(INT_0, 1);
		//noteMtrl->SetScalar(FLOAT_0, 9999.f);
		SpriteRenderer()->SetMaterial(noteMtrl);
		//	SpriteRenderer()->CreateDynamicMaterial();
	}
	else
	{
		SpriteRenderer()->SetMaterial(LOAD(AMaterial, L"Material\\Notes.mtrl"));
	}

	GetOwner()->SetActive(false);
}

void CNote::Tick()
{
	Vec3 pos = Transform()->GetRelativePosition();

	// 여기선 화면 밖으로 사라졌을 경우 Destroy만 처리.
	float worldY = Transform()->GetWorldPos().y;
	if (worldY >= 700.f)
	{
		Destroy();
	}

	pos.y += DT * m_speed;

	Transform()->SetRelativePosition(pos);

}

void CNote::SaveToSceneFile(FILE* _File)
{
}

void CNote::LoadFromSceneFile(FILE* _File)
{
}

void CNote::ActivateVanish()
{
	if (m_isBody)
		SpriteRenderer()->GetMaterial()->SetScalar(INT_0, 0);
}
