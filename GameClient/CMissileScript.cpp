#include "pch.h"
#include "CMissileScript.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "CCollider2D.h"
#include "TaskManager.h"

CMissileScript::CMissileScript(Ptr<GameObject> obj)
	: m_speed(300.f), m_targetObj(obj)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	TaskInfo info = {};

	info.type = TASK_TYPE::DESTROY_OBJECT;
	info.param_0 = (DWORD_PTR)GetOwner();

	TaskManager::GetInstance()->AddTask(info);

	m_targetObj->m_dotween->DOShakePosition(0.2f);
}

void CMissileScript::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CMissileScript::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CMissileScript::Begin()
{
	Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CMissileScript::BeginOverlap);
	Collider2D()->AddDynamicOverlap(this, (COLLISION_EVENT)&CMissileScript::Overlap);
	Collider2D()->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CMissileScript::EndOverlap);
}

void CMissileScript::Tick()
{
	//if (IsValid())
	Vec3 vPos = Transform()->GetRelativePosition();
	
	Vec3 dir = m_targetObj->Transform()->GetRelativePosition() - vPos;
	dir = Vec3(dir.x, dir.y, 0.f);
	dir.Normalize();

	Vec3 upVec = Vec3(0.f, 1.f, 0.f);

	float dot = dir.Dot(upVec);

	float degree = acosf(dot);

	vPos += dir * m_speed * DT;

	Transform()->SetRelativePosition(vPos);
	Transform()->SetRelativeRotation(Vec3(0.f, 0.f, degree));
	//Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
}