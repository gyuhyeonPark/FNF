#pragma once
#include "CScript.h"
class CCameraMove : public CScript
{
public:
	CCameraMove();
	virtual ~CCameraMove();

public:
	virtual void Tick() override;

private:
	void MovePerspective();
	void MoveOrthographic();

private:
	float m_speed;
};

