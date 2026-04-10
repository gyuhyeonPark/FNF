#include "pch.h"
#include "CTransform.h"
#include "GameObject.h"

CTransform::CTransform()
	:Component(COMPONENT_TYPE::TRANSFORM), m_relativeScale(Vec3(1.f, 1.f, 1.f)), m_independentScale(false)
{
	m_dir[(UINT)DIR::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_dir[(UINT)DIR::UP] = Vec3(0.f, 1.f, 0.f);
	m_dir[(UINT)DIR::FRONT] = Vec3(0.f, 0.f, 1.f);
}

CTransform::~CTransform()
{
}

Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = m_relativeScale;
	if (m_independentScale)
		return vWorldScale;

	Ptr<GameObject> pParent = GetOwner()->GetParent();
	while (nullptr != pParent)
	{
		vWorldScale *= pParent->GetTransform()->GetRelativeScale();

		if (pParent->GetTransform()->m_independentScale)
			break;

		pParent = pParent->GetParent();
	}

	return vWorldScale;
}

void CTransform::FinalTick()
{
	// 크기 -> 회전 -> 이동
	Matrix matTrans = XMMatrixTranslation(m_relativePos.x, m_relativePos.y, m_relativePos.z);

	Matrix matScale = XMMatrixScaling(m_relativeScale.x, m_relativeScale.y, m_relativeScale.z);

	Matrix matRot = XMMatrixRotationX(m_relativeRot.x)
		* XMMatrixRotationY(m_relativeRot.y)
		* XMMatrixRotationZ(m_relativeRot.z);

	m_dir[(UINT)DIR::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_dir[(UINT)DIR::UP] = Vec3(0.f, 1.f, 0.f);
	m_dir[(UINT)DIR::FRONT] = Vec3(0.f, 0.f, 1.f);

	// 변환 행렬 적용 대생을 위치 벡터로 취급. w = 1
	//XMVector3TransformCoord(m_dir[(UINT)DIR::RIGHT], m_rotMat);

	// 방향성 벡터로 취급. w = 0
	m_dir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(m_dir[(UINT)DIR::RIGHT], matRot);
	m_dir[(UINT)DIR::UP] = XMVector3TransformNormal(m_dir[(UINT)DIR::UP], matRot);
	m_dir[(UINT)DIR::FRONT] = XMVector3TransformNormal(m_dir[(UINT)DIR::FRONT], matRot);

	//m_matWorld = Matrix::Identity;
	m_matWorld = matScale * matRot * matTrans;

	if (GetOwner()->GetParent() != nullptr)
	{
		if (m_independentScale)
		{
			Vec3 parentScale = GetOwner()->GetParent()->GetTransform()->GetWorldScale();
			Matrix matParentScale = XMMatrixScaling(parentScale.x, parentScale.y, parentScale.z);
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			m_matWorld = m_matWorld * matParentScaleInv * GetOwner()->GetParent()->GetTransform()->GetWorldMat();
		}
		else
			m_matWorld *= GetOwner()->GetParent()->GetTransform()->GetWorldMat();
	}
}

void CTransform::Binding()
{
	g_Trans.srtMat = m_matWorld;
	g_Trans.uv = m_uv;
	g_Trans.isRotated = m_isRotated;
	Graphics::GetInstance()->GetConstantBuffer(CB_TYPE::TRANSFORM)->SetData(&g_Trans);
	Graphics::GetInstance()->GetConstantBuffer(CB_TYPE::TRANSFORM)->Binding();
}

void CTransform::SaveToSceneFile(FILE* _File)
{
	fwrite(&m_relativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_relativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_relativeRot, sizeof(Vec3), 1, _File);
	fwrite(&m_independentScale, sizeof(bool), 1, _File);
}

void CTransform::LoadFromSceneFile(FILE* _File)
{
	fread(&m_relativePos, sizeof(Vec3), 1, _File);
	fread(&m_relativeScale, sizeof(Vec3), 1, _File);
	fread(&m_relativeRot, sizeof(Vec3), 1, _File);
	fread(&m_independentScale, sizeof(bool), 1, _File);
}
