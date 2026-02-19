#ifndef _STRUCT
#define _STRUCT

struct Light2DInfo
{
    int Type;
    float3 Color;
    float3 Ambient; // 환경광. 광원이 존재하면서 최소한으로 발생하는 빛의 세기
    float3 LightDir;
    float3 WorldPos;
    float Radius;
    float Angle;
};

#endif