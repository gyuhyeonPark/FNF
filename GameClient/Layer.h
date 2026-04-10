#pragma once
#include "Entity.h"

class Layer : public Entity
{
public:
	Layer();
	Layer(const Layer& _Orgin);
	virtual ~Layer();

public:
	void AddObject(Ptr<GameObject> obj);
	void RegisterObject(Ptr<GameObject> obj) { m_allObjVec.push_back(obj); }
	void DeregisterObject() { m_allObjVec.clear(); }
	void DeregisterAsParent(Ptr<GameObject> obj);

	const vector<Ptr<GameObject>>& GetParentObjects() { return m_parentVec; }
	const vector<Ptr<GameObject>>& GetAllObjects() { return m_allObjVec; }

	void Begin();
	void Tick();
	void FinalTick();
	void Render();

private:
	vector<Ptr<GameObject>> m_parentVec;		// Layer에 소속된 최상위 부모타입 오브젝트들
	vector<Ptr<GameObject>> m_allObjVec;		// Layer에 소속된 모든(부모, 자식) 오브젝트
	int						m_layerIdx;			// Layer 본인의 인덱스 (0 ~ 31)

	friend class AScene;
};

