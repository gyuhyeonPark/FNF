#pragma once
#include "Component.h"
#include "GameObject.h"

/// <summary>
/// 사용자가 정의하여 동작하는 컴포넌트 인터페이스
/// </summary>
class CScript : public Component
{
public:
	CScript();
	virtual ~CScript();

public:
	virtual void Tick() = 0;
	virtual void FinalTick() final {}
	virtual void Destroy();
};

