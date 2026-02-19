#pragma once

struct VertexInfo
{
	Vec3 position;
	Vec2 uv;
	Vec4 color;
};

struct DbgInfo
{
	DBG_SHAPE shape;
	Vec3 pos;
	Vec3 scale;
	Vec3 rotation;
	Matrix matWorld;
	Vec4 color;
	float elapsed;		// 경과 시간
	float duration;		// 수명
	bool depthTest;
};

struct TaskInfo
{
	TASK_TYPE type;
	DWORD_PTR param_0;
	DWORD_PTR param_1;
	DWORD_PTR param_2;
};


struct Light2DInfo
{
	LIGHT_TYPE	Type;
	Vec3		Color;
	Vec3		Ambient;	// 환경광. 광원이 존재하면서 최소한으로 발생하는 빛의 세기
	Vec3		LightDir;
	Vec3		WorldPos;
	float		Radius;
	float		Angle;	
};

struct ConstantStruct
{
	Matrix srtMat;
	Matrix matView;
	Matrix matProj;
	Vec4 uv;
	bool isRotated;
	Vec3 padding;
};

extern ConstantStruct g_Trans;

struct MtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	mat[2];
};

struct GlobalData
{
	Vec2	Resolution;		// 화면 해상도
	int		Light2DCount;	// 2D 광원 개수
	int		Light3DCount;	// 3D 광원 개수
	float	DeltaTime;		// DeltaTime
	float	Time;			// 누적 시간값
	float   EngineDT;		// DeltaTime
	float	EngineTime;		// 누적 시간값
};
extern GlobalData g_Global;