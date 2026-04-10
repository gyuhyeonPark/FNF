#pragma once
#include "Component.h"
class CLight2D : public Component
{
public:
	CLight2D();
	virtual ~CLight2D();

public:
	virtual void FinalTick() override;

public:
    const Light2DInfo& GetInfo() { return m_info; }

    LIGHT_TYPE GetLightType() { return m_info.Type; }
    void SetLightType(LIGHT_TYPE _Type) { m_info.Type = _Type; }

    Vec3 GetLightColor() { return m_info.Color; }
    void SetLightColor(Vec3 _Color) { m_info.Color = _Color; }

    Vec3 GetLightDir() { return m_info.LightDir; }
    void SetLightDir(Vec3 _dir) { m_info.LightDir = _dir; }

    Vec3 GetAmbient() { return m_info.Ambient; }
    void SetAmbient(Vec3 _Ambient) { m_info.Ambient = _Ambient; }

    float GetRadius() { return m_info.Radius; }
    void SetRadius(float _R) { m_info.Radius = _R; }

    float GetAngle() { return m_info.Angle; }
    void SetAngle(float _Angle) { m_info.Angle = _Angle; }

public:
    CLONE(CLight2D);

    virtual void SaveToSceneFile(FILE* _File) override;
    virtual void LoadFromSceneFile(FILE* _File) override;

private:
	Light2DInfo m_info;
};

