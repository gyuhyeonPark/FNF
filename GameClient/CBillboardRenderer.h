#pragma once
#include "CRenderComponent.h"

class CBillboardRenderer : public CRenderComponent
{
public:
    CBillboardRenderer();
    virtual ~CBillboardRenderer();

public:
    GET_SET(Vec2, BillboardScale);

public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override;

private:
    Vec2    m_BillboardScale;
    //Ptr<ATexture> m_tex;
};

