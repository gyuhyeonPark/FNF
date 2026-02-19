#pragma once
#include "Asset.h"
#include "Layer.h"

class AScene : public Asset
{
public:
	AScene();
	virtual ~AScene();

public:
	void AddObject(int idx, Ptr<GameObject> obj);
	void Deregister();
	Layer* GetLayer(int idx) { assert(idx >= 0 && idx < MAX_LAYER); return &m_layerArr[idx]; }

public:
	void Begin();
	void Tick();
	void FinalTick();

	void CheckCollisionLayer(UINT layerIdx1, UINT layerIdx2);
	void CheckCollisionLayer(const wstring& layerName1, const wstring& layerName2);

	UINT* GetCollisionMatrix() { return m_matrix; }

	Ptr<GameObject> FindObjectByName(const wstring& name);

private:
	Layer m_layerArr[MAX_LAYER];		// 하나의 Scene마다 32개의 레이어가 존재.

	// 어떤 레이어끼리 충돌검사를 진행할 지 마킹한 데이터.
	// 행렬 중 y = x 그래프 기준 좌측 부분만 사용
	// -> 작은 layer를 행으로 사용
	UINT m_matrix[MAX_LAYER];			// 32(UINT, bit) X 32(MAXLAYER) 의 2차원 행렬로 사용
};

