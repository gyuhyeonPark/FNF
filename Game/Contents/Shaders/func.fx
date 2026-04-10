#ifndef _FUNC
#define _FUNC

#include "value.hlsl"

float3 CalcLight2D(int _lightIdx, float3 _pixelPos)
{
    float3 lightColor = float3(0.f, 0.f, 0.f);
    
    float distRatio = 1.f;
    
    if (g_Light2D[_lightIdx].Type == 0)
    {
        // Directional
        lightColor = g_Light2D[_lightIdx].Color + g_Light2D[_lightIdx].Ambient;
    }
    else if (g_Light2D[_lightIdx].Type == 1)
    {
        lightColor = g_Light2D[_lightIdx].Color;
        
        // Point Light
        float3 dirVec = _pixelPos - g_Light2D[_lightIdx].WorldPos;
        dirVec.z = 0;
        float distance = length(dirVec);
        if (distance >= g_Light2D[_lightIdx].Radius)
            distRatio = 0.f;
        else
            //distRatio = saturate(1 - (distance / g_Light2D[_lightIdx].Radius));
            //distRatio = saturate(cos(distance * ((PI / 2.f) / g_Light2D[_lightIdx].Radius)));
            distRatio = saturate(cos(saturate(distance / g_Light2D[_lightIdx].Radius) * (PI / 2.f)));
    }
    else if (g_Light2D[_lightIdx].Type == 2)
    {
        // Spot Light
        lightColor = g_Light2D[_lightIdx].Color;
        
        // Point Light
        float3 dirVec = _pixelPos - g_Light2D[_lightIdx].WorldPos;
        dirVec.z = 0;
        float distance = length(dirVec);
        if (distance >= g_Light2D[_lightIdx].Radius)
            distRatio = 0.f;
        else
            distRatio = saturate(cos(saturate(distance / g_Light2D[_lightIdx].Radius) * (PI / 2.f)));
        
        // 부채꼴 계산 - dirVec.normalized 와 gLight.dir.normalized 내적
        float3 lightDir = g_Light2D[_lightIdx].LightDir;
        lightDir.z = 0;
        float3 nLightDir = normalize(lightDir);
        float3 ndirVec = normalize(dirVec);
        
        float dotted = dot(nLightDir, ndirVec);
        float degree = degrees(acos(dotted));
        
        if (degree >= g_Light2D[_lightIdx].Angle / 2.f || degree <= g_Light2D[_lightIdx].Angle / 2.f * -1.f)
            distRatio = 0.f;
    }
    
    return lightColor * distRatio;
}

#endif