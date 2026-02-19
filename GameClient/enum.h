#pragma once

enum class ASSET_TYPE
{
	MESH,
	TEXTURE,
	SOUND,
	GRAPHICSHADER,
	MATERIAL,
	SCENE,
	SPRITE,
	FLIPBOOK,
	TILEMAP,

	END,
};

enum class COMPONENT_TYPE
{
	TRANSFORM,
	CAMERA,
	COLLIDER2D,
	LIGHT2D,
	LIGHT3D,

	MESHRENDERER,
	BILLBOARD_RENDER,
	FLIPBOOK_RENDERER,
	PARTICLE_RENDER,
	TILE_RENDERER,
	SPRITE_RENDERER,

	END,

	SCRIPT,
};

enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2
	
	END,
};

enum class BS_TYPE
{
	DEFAULT,		// (SrcRGB * 1) + (DestRGB * 0)
	ALPHABLEND,		// (SrcRGB * SrcA) + (DestRGB * (1 - SrcA))
	ONE_ONE,		// (SrcRGB * 1) + (DestRGB * 1)
	END,
};

enum class RS_TYPE
{
	CULL_BACK,	// 뒷면( 정점 반시계방향으로.. )
	CULL_FRONT, // 앞면
	CULL_NONE,  // Culling X
	WIRE_FRAME, // 정점과 정점 사이를 잇는 부분을 제외

	END,
};

enum class DS_TYPE
{
	LESS,				// 깊이판정 LESS,		 성공 시 - 깊이쓰기 O
	LESS_EQUL,			// 깊이판정 LESS_EQUAL
	NO_TEST,			// 깊이판정 X(무조건 성공) 성공 시 - 깊이쓰기 O
	NO_TEST_NO_WRITE,	// 깊이판정 X(무조건 성공) 성공 시 - 깊이쓰기 X

	END,
};

enum class DIR
{
	RIGHT,
	UP,
	FRONT,

	END,
};

enum TEX_PARAM
{
	TEX_0, // t0
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEX_END,
};

enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
};

enum class DBG_SHAPE
{
	RECT,
	CIRCLE,

	CUBE,
	SPHERE,
};

enum class TASK_TYPE
{
	CREATE_OBJECT,
	DESTROY_OBJECT,

};

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// 방향성 - 멀리서부터 오는 광원에게 설정 (빛이 월드로 오는 방향이 같다고 판정)
	POINT,			// 정광원 - 기본적인 광원, 한 점으로부터 주변으로 빛이 뻗어나감
	SPOT,			// 스포트 - 특수한 광원, 빛을 특정 방향으로 모아서 보내는 것으로 봄
};