#pragma once
#include "Component.h"

class CScript;
class CCollider2D;
typedef void(CScript::* COLLISION_EVENT)(CCollider2D*, CCollider2D*);

struct COLLISION_DELEGATE
{
	CScript* inst;
	COLLISION_EVENT memFunc;
};

class CCollider2D : public Component
{
public:
	CCollider2D();
	virtual ~CCollider2D();

	virtual void FinalTick() override;

public:
	GET_SET(Vec2, Offset);
	GET_SET(Vec2, Scale);

	const Matrix& GetWorldMat() { return m_worldMat; }

	void BeginOverlap(Ptr<CCollider2D> other);
	void Overlap(Ptr<CCollider2D> other);
	void EndOverlap(Ptr<CCollider2D> other);
	
public:
	void AddDynamicBeginOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc);
	void AddDynamicOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc);
	void AddDynamicEndOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc);

private:
	Vec2 m_Scale;
	Vec2 m_Offset;
	Matrix m_worldMat;

	int m_overlapCnt;

	vector<COLLISION_DELEGATE> m_vecBeginDel;
	vector<COLLISION_DELEGATE> m_vecOverDel;
	vector<COLLISION_DELEGATE> m_vecEndDel;


	friend class CollisionManager;
};

