#pragma once
#include "CScript.h"

class CPlayerScript :
    public CScript
{
private:


public:
    virtual void Tick() override;

private:
    void Move();
    void Shoot();

public:
    CPlayerScript();
    virtual ~CPlayerScript();
};

