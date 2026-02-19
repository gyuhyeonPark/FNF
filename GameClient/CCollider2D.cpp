#include "pch.h"
#include "CCollider2D.h"

#include "RenderManager.h"
#include "CScript.h"

CCollider2D::CCollider2D()
	:Component(COMPONENT_TYPE::COLLIDER2D), m_Scale(Vec2(1.f, 1.f)), m_worldMat{}, m_overlapCnt(0)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::FinalTick()
{
	Matrix matTran = XMMatrixTranslation(m_Offset.x, m_Offset.y, 0.f);
	
	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, 0.f);

	m_worldMat = matScale * matTran;
	m_worldMat *= Transform()->GetWorldMat();

	assert(m_overlapCnt >= 0);
	if (m_overlapCnt > 0)
		DrawDebugRect(m_worldMat, Vec4(1.f, 0.f, 0.f, 1.f), 0.f);
	else if (m_overlapCnt == 0)
		DrawDebugRect(m_worldMat, Vec4(0.f, 1.f, 0.f, 1.f), 0.f);
}

void CCollider2D::BeginOverlap(Ptr<CCollider2D> other)
{
	for (size_t i = 0; i < m_vecBeginDel.size(); i++)
	{
		(m_vecBeginDel[i].inst->*m_vecBeginDel[i].memFunc)(this, other.Get());
	}
	++m_overlapCnt;
}

void CCollider2D::Overlap(Ptr<CCollider2D> other)
{
	for (size_t i = 0; i < m_vecOverDel.size(); i++)
	{
		(m_vecOverDel[i].inst->*m_vecOverDel[i].memFunc)(this, other.Get());
	}
}

void CCollider2D::EndOverlap(Ptr<CCollider2D> other)
{
	for (size_t i = 0; i < m_vecEndDel.size(); i++)
	{
		(m_vecEndDel[i].inst->*m_vecEndDel[i].memFunc)(this, other.Get());
	}
	--m_overlapCnt;
}

void CCollider2D::AddDynamicBeginOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc)
{
	m_vecBeginDel.push_back(COLLISION_DELEGATE{ _Inst , _MemFunc });
}

void CCollider2D::AddDynamicOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc)
{
	m_vecOverDel.push_back(COLLISION_DELEGATE{ _Inst , _MemFunc });
}

void CCollider2D::AddDynamicEndOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc)
{
	m_vecEndDel.push_back(COLLISION_DELEGATE{ _Inst , _MemFunc });
}
