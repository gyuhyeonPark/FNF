#pragma once
#include "Component.h"
#include "AMesh.h"
#include "AMaterial.h"

class CRenderComponent : public Component
{
public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Origin);
    virtual ~CRenderComponent();

public:
    GET_SET(Ptr<AMesh>, Mesh);
    virtual void SaveToSceneFile(FILE* _File) override;
    virtual void LoadFromSceneFile(FILE* _File) override;

    void SetMaterial(Ptr<AMaterial> _mtrl);
    Ptr<AMaterial> GetMaterial() { return m_Mtrl; }
    Ptr<AMaterial> GetSharedMaterial();

public:
    virtual void Init() override;
    virtual void Render() = 0;
    virtual void CreateMaterial() {};

    Ptr<AMaterial> CreateDynamicMaterial();

private:
    Ptr<AMesh>          m_Mesh;
    
    Ptr<AMaterial>      m_Mtrl;             // 현재 사용중인 재질
    Ptr<AMaterial>      m_SharedMtrl;       // 공유 재질(애셋 매니저의 관리를 받는..)
    Ptr<AMaterial>      m_DynamicMtrl;      // 나만의 동적 재질(쓰고 버리는..)
};

