#include "pch.h"
#include "CEditorCamMoveScript.h"

#include "KeyManager.h"
#include "TimeManager.h"
#include "CTransform.h"
#include "CCamera.h"

void CEditorCamMoveScript::SaveToSceneFile(FILE* _File)
{
}

void CEditorCamMoveScript::LoadFromSceneFile(FILE* _File)
{
}

CEditorCamMoveScript::CEditorCamMoveScript()
	: CScript(-1), m_Speed(500.f)
{
}

CEditorCamMoveScript::~CEditorCamMoveScript()
{
}

void CEditorCamMoveScript::Tick()
{
	if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
		MovePerspective();
	else
		MoveOrthographic();
}

void CEditorCamMoveScript::MovePerspective()
{
	Vec3 vPos = Transform()->GetRelativePosition();
	Vec3 vRot = Transform()->GetRelativeRotation();

	Vec3 vFront = Transform()->GetDir(DIR::FRONT);
	Vec3 vRight = Transform()->GetDir(DIR::RIGHT);

	float Speed = m_Speed;

	if (KEY_PRESSED(KEY::LSHIFT))
		Speed *= 3.f;


	if (1 == KeyManager::GetInstance()->GetMouseWheel())
		vPos += vFront * 10.f;
	if (-1 == KeyManager::GetInstance()->GetMouseWheel())
		vPos -= vFront * 10.f;


	if (KEY_PRESSED(KEY::W))
		vPos += vFront * Speed * E_DT;
	if (KEY_PRESSED(KEY::S))
		vPos -= vFront * Speed * E_DT;
	if (KEY_PRESSED(KEY::A))
		vPos -= vRight * Speed * E_DT;
	if (KEY_PRESSED(KEY::D))
		vPos += vRight * Speed * E_DT;

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vMouseDir = KeyManager::GetInstance()->GetmouseDir();
		vRot.y += vMouseDir.x * E_DT * XM_2PI * 3.f;
		vRot.x += vMouseDir.y * E_DT * XM_2PI * 3.f;
	}

	Transform()->SetRelativePosition(vPos);
	Transform()->SetRelativeRotation(vRot);
}

void CEditorCamMoveScript::MoveOrthographic()
{
	Vec3 vPos = Transform()->GetRelativePosition();

	float Speed = m_Speed;
	if (KEY_PRESSED(KEY::LSHIFT))
		Speed *= 3.f;


	if (KEY_PRESSED(KEY::W))
		vPos.y += E_DT * Speed;
	if (KEY_PRESSED(KEY::S))
		vPos.y -= E_DT * Speed;
	if (KEY_PRESSED(KEY::A))
		vPos.x -= E_DT * Speed;
	if (KEY_PRESSED(KEY::D))
		vPos.x += E_DT * Speed;

	Transform()->SetRelativePosition(vPos);
	Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
}
