#pragma once
#include "Component.h"
#include "GameObject.h"

#include "Source/ScriptMgr.h"

/// <summary>
/// 사용자가 정의하여 동작하는 컴포넌트 인터페이스
/// </summary>
class CScript : public Component
{
public:
	CScript(int _ScriptType);
	CScript(const CScript& _Origin);
	virtual ~CScript();

public:
	virtual void Tick() = 0;
	virtual void FinalTick() final {}
	virtual void Destroy();

public:
	virtual CScript* Clone() = 0;

public:
	int GetScriptType() { return m_ScriptType; }

private:
	int m_ScriptType;
};

#define ADD_DYNAMIC_BEGIN_OVERLAP(MemFunc) Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&MemFunc);
#define ADD_DYNAMIC_OVERLAP(MemFunc) Collider2D()->AddDynamicOverlap(this, (COLLISION_EVENT)&MemFunc);
#define ADD_DYNAMIC_END_OVERLAP(MemFunc) Collider2D()->AddDynamicEndOverlap(this, (COLLISION_EVENT)&MemFunc);
