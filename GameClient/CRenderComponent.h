#pragma once
#include "Component.h"
#include "AMesh.h"
#include "AMaterial.h"

class CRenderComponent : public Component
{
public:
    CRenderComponent(COMPONENT_TYPE _Type);
    virtual ~CRenderComponent();

public:
    GET_SET(Ptr<AMesh>, Mesh);
    GET_SET(Ptr<AMaterial>, Mtrl);

public:
    virtual void Init() override;
    virtual void Render() = 0;
    virtual void CreateMaterial() {};

private:
    Ptr<AMesh>          m_Mesh;
    Ptr<AMaterial>      m_Mtrl;
};

