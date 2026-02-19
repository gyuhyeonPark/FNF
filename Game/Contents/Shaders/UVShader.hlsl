// VertexShader.hlsl

struct VSInput
{
    float3 pos : POSITION; // offset 0
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 pos : SV_POSITION;   // 래스터라이저로 보낼 때의 NDC 좌표
    float2 uv : TEXCOORD;
};

cbuffer c : register(b0)
{
    // { u0, u1, v0, v1 }
    row_major matrix worldMat;
    row_major matrix viewMat;
    row_major matrix projMat;
    float4 offsetUV;
    bool isRotated;
    float3 padding;
}

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    output.pos = mul(float4(input.pos, 1.f), worldMat);
    output.pos = mul(output.pos, viewMat);
    output.pos = mul(output.pos, projMat);
    output.uv = input.uv;
    return output;
}

Texture2D Texture0 : register(t0);
SamplerState Sampler0 : register(s0);
SamplerState Sampler1 : register(s1);

float4 PSMain(VSOutput input) : SV_TARGET
{
    float4 vColor = Texture0.Sample(Sampler0, input.uv);

    if (vColor.r < 0.5f && vColor.g < 0.2f && vColor.b < 0.2f)
        discard;
    
    return Texture0.Sample(Sampler0, input.uv);
}

float4 PSMain2(VSOutput input) : SV_TARGET
{
    return float4(1.f, 0.f, 0.f, 1.0f);
}