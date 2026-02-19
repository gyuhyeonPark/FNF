// VertexShader.hlsl

struct VSInput
{
    float3 pos : POSITION; // offset 0
    float3 color : COLOR; // offset 12
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    // 이미 NDC 공간이라고 가정 (-1 ~ 1)
    output.pos = float4(input.pos, 1.0f);

    output.color = input.color;
    return output;
}


float4 PSMain(PSInput input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}
