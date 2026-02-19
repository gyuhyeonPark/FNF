#pragma once
#include "CScript.h"
class CMissileScript :
    public CScript
{
public:
    virtual void Begin();
    void Tick();

public:
    CMissileScript(Ptr<GameObject> obj);
    virtual ~CMissileScript();

    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

private:
    float m_speed;
    Ptr<GameObject> m_targetObj;
};

