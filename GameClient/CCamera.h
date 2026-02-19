#pragma once
#include "Component.h"

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

class CCamera : public Component
{
public:
	CCamera();
	virtual ~CCamera();

public:
	virtual void Begin() override;
	virtual void FinalTick() override;

public:
	void LayerCheckAll() { m_layerCheck = 0xffffffff; }
	void LayerCheckClear() { m_layerCheck = 0; }
	void LayerCheck(int idx);

	void SetLayerCheck(UINT flag) { m_layerCheck = flag; }

public:
	void Render();

public:
	GET_SET(PROJ_TYPE, ProjType);
	GET_SET(float, Far);
	GET_SET(float, Width);
	GET_SET(float, AspectRatio);
	GET_SET(float, OrthoScale);
	//GET_SET(Matrix, MatView)
	//GET_SET(Matrix, MatProj)

	UINT GetCurrentCheckedLayer() { return m_layerCheck; }

	float GetFOV() { return XMConvertToDegrees(m_Fov); }
	void SetFOV(float degree)
	{
		m_Fov = XMConvertToRadians(degree);
	}

private:
	UINT m_layerCheck;		// 렌더링 할 레이어 비트 체크
	PROJ_TYPE m_ProjType;

	float m_Far;
	float m_Width;
	float m_AspectRatio;	// 종횡비

	float m_Fov;			// 원근투영 시야각

	float m_OrthoScale;		// 직교 투영 배율

	Matrix m_MatView;		// view 행렬
	Matrix m_MatProj;		// Projection 행렬
};

