#ifndef _TEST
#define _TEST
#include "value.hlsl"

struct VSInput
{
    float3 pos : POSITION; // offset 0
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 pos : SV_POSITION;   // 래스터라이저로 보낼 때의 NDC 좌표
    float2 uv : TEXCOORD;
    float3 worldPos : TEXCOORD1;
};

VSOutput VSMainAnim(VSInput input)
{
    VSOutput output;
    
    float4 vWorld = mul(float4(input.pos, 1.f), g_matWorld);
    //vWorld.xy += g_vec2_0;
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vView.xyz += input.pos * 100.f;

    output.pos = vProj;
    output.worldPos = vWorld;
    
    // uv lerp
    float2 uv;

    if (!isRotated)
    {
    // 일반
        uv.x = lerp(offsetUV.x, offsetUV.y, input.uv.x);
        uv.y = lerp(offsetUV.z, offsetUV.w, input.uv.y);
    }
    else
    {
        // 오른쪽으로 누워있음 → 좌측으로 90도 회전 (CCW)
        float2 ruv;
        ruv.x = 1.0f - input.uv.y; // 1 - v
        ruv.y = input.uv.x; // u

        uv.x = lerp(offsetUV.x, offsetUV.y, ruv.x);
        uv.y = lerp(offsetUV.z, offsetUV.w, ruv.y);
    }

    output.uv = uv;

    return output;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
    float4 vColor = g_tex_0.Sample(g_sam_0, input.uv);
    
    if (vColor.a < 0.1f)
        discard;
    
    // 광원 적용X
    
    return vColor;
}
#endif