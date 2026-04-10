#include "pch.h"
#include "CCameraMove.h"
#include "CCamera.h"

CCameraMove::CCameraMove()
	: CScript(GetScriptType()), m_speed(1.f), m_beatHeat(false), m_zoomRatio(1.3f), m_initZoomRatio(0.f)
{
}

CCameraMove::~CCameraMove()
{
}

void CCameraMove::Begin()
{
	m_initZoomRatio = GetOwner()->Camera()->GetOrthoScale();
}

void CCameraMove::Tick()
{
	if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
		MovePerspective();
	else
		MoveOrthographic();

	if (m_beatHeat)
		ZoomBeat();
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

void CCameraMove::ZoomBeat()
{
	m_elapsed += DT;
	// m_elapsed ~ m_zoomDuration -> 1 ~ m_zoomRatio
	if (m_elapsed <= m_zoomDuration)
	{
		float ratio = lerp(m_initZoomRatio, m_zoomRatio, m_elapsed / m_zoomDuration);
		GetOwner()->Camera()->SetOrthoScale(ratio);
	}
	else
	{
		// return logic
		float elapsed = m_elapsed - m_zoomDuration;

		if (elapsed <= m_returnDuration)
		{
			float ratio = lerp(m_zoomRatio, m_initZoomRatio, elapsed / m_returnDuration);
			GetOwner()->Camera()->SetOrthoScale(ratio);
		}
		else
		{
			m_beatHeat = false;
			m_elapsed = 0.f;
		}
	}
}

void CCameraMove::BeatHeat(float _zoomDuration, float _returnDuration, float _zoomRatio)
{
	m_beatHeat = true;
	m_zoomDuration = _zoomDuration;
	m_returnDuration = _returnDuration;
	m_zoomRatio = _zoomRatio;
}

void CCameraMove::SaveToSceneFile(FILE* _File)
{
}

void CCameraMove::LoadFromSceneFile(FILE* _File)
{
}
