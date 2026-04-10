#pragma once
#include "CScript.h"
class CMissileScript :
    public CScript
{
public:
    virtual void Begin();
    void Tick();

public:
    CMissileScript();
    virtual ~CMissileScript();

    void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

    CLONE(CMissileScript);

    virtual void SaveToSceneFile(FILE* _File) override;
    virtual void LoadFromSceneFile(FILE* _File) override;

public:
    void SetTargetObj(Ptr<GameObject> obj) { m_targetObj = obj; }

private:
    float m_speed;
    Ptr<GameObject> m_targetObj;
};

