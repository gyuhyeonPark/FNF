#include "pch.h"
#include "CComboImage.h"
#include "GameObject.h"
#include "CDotween.h"

CComboImage::CComboImage()
	: CScript(SCRIPT_TYPE::COMBOIMAGE), m_destinaion{},
	m_elapsed(0.f), m_duration(1.f)
{
}

CComboImage::~CComboImage()
{
}

void CComboImage::Begin()
{
	// 도착 지점 설정
	Vec3 offset = Vec3(60.f, 40.f, 1.f);
	m_destinaion = Transform()->GetRelativePosition() + offset;
	GetOwner()->GetScript<CDotween>()->DOMove(m_destinaion, m_duration)
		->SetEase(EaseOutQuint)->OnComplete([this]()
		{
			Destroy();
		});
}

void CComboImage::Tick()
{
	m_elapsed += DT;

	GetOwner()->MeshRenderer()->GetMaterial()->SetScalar(FLOAT_0, m_elapsed - 0.3f);
	GetOwner()->MeshRenderer()->GetMaterial()->SetScalar(FLOAT_1, m_duration);
}

void CComboImage::SaveToSceneFile(FILE* _File)
{
}

void CComboImage::LoadFromSceneFile(FILE* _File)
{
}
