#include "pch.h"
#include "CCameraMove.h"
#include "CCamera.h"

CCameraMove::CCameraMove()
	:m_speed(1.f)
{
}

CCameraMove::~CCameraMove()
{
}

void CCameraMove::Tick()
{
	if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
		MovePerspective();
	else
		MoveOrthographic();
}

void CCameraMove::MovePerspective()
{
	Vec3 vPos = GetOwner()->GetTransform()->GetRelativePosition();
	Vec3 rot = GetOwner()->GetTransform()->GetRelativeRotation();

	Vec3 vFront = GetOwner()->GetTransform()->GetDir(DIR::FRONT);
	Vec3 vRight = GetOwner()->GetTransform()->GetDir(DIR::RIGHT);

	if (KEY_PRESSED(KEY::W))
		vPos += vFront * 500.f * DT;
	if (KEY_PRESSED(KEY::S))
		vPos -= vFront * 500.f * DT;
	if (KEY_PRESSED(KEY::A))
		vPos -= vRight * 500.f * DT;
	if (KEY_PRESSED(KEY::D))
		vPos += vRight * 500.f * DT;

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vMouseDir = KeyManager::GetInstance()->GetmouseDir();
		rot.y += vMouseDir.x * DT * XM_2PI * 3.f;
		rot.x += vMouseDir.y * DT * XM_2PI * 3.f;
	}

	if (1 == KeyManager::GetInstance()->GetMouseWheel())
		vPos += vFront * 10.f;
	if (-1 == KeyManager::GetInstance()->GetMouseWheel())
		vPos -= vFront * 10.f;

	GetOwner()->GetTransform()->SetRelativePosition(vPos);
	GetOwner()->GetTransform()->SetRelativeRotation(rot);
}

void CCameraMove::MoveOrthographic()
{

	Vec3 vPos = GetOwner()->GetTransform()->GetRelativePosition();
	Vec3 rot = GetOwner()->GetTransform()->GetRelativeRotation();

	Vec3 vUp = GetOwner()->GetTransform()->GetDir(DIR::UP);
	Vec3 vRight = GetOwner()->GetTransform()->GetDir(DIR::RIGHT);

	if (KEY_PRESSED(KEY::W))
		vPos += vUp * 500.f * DT;
	if (KEY_PRESSED(KEY::S))
		vPos -= vUp * 500.f * DT;
	if (KEY_PRESSED(KEY::A))
		vPos -= vRight * 500.f * DT;
	if (KEY_PRESSED(KEY::D))
		vPos += vRight * 500.f * DT;

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vMouseDir = KeyManager::GetInstance()->GetmouseDir();
		rot.y += vMouseDir.x * DT * XM_2PI * 3.f;
		rot.x += vMouseDir.y * DT * XM_2PI * 3.f;
	}

	if (1 == KeyManager::GetInstance()->GetMouseWheel())
		vPos += vUp * 10.f;
	if (-1 == KeyManager::GetInstance()->GetMouseWheel())
		vPos -= vUp * 10.f;

	GetOwner()->GetTransform()->SetRelativePosition(vPos);
	GetOwner()->GetTransform()->SetRelativeRotation(rot);
}
