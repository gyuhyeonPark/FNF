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

	bool IsChanged()
	{
		bool Changed = m_Changed;
		m_Changed = false;
		return Changed;
	}

	void SetChanged() { m_Changed = true; }

public:
	AScene* Clone() { return new AScene(*this); }

	virtual int Save(const wstring& _FilePath) override;
	virtual int Load(const wstring& _FilePath) override;

private:
	Layer m_layerArr[MAX_LAYER];		// 하나의 Scene마다 32개의 레이어가 존재.
	UINT m_matrix[MAX_LAYER];			// 어떤 레이어와, 어떤 레이어가 충돌검사를 진행할지 마킹한 데이터
	bool m_Changed;						// 레벨 안에 오브젝트들의 상태가 변경(오브젝트 추가, 삭제, 계층구조 변경) 됐는지 확인하는 변수
};

