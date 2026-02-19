#pragma once
#include "Component.h"
#include "components.h"
#include "AScene.h"
#include "Dotween.h"

#define GET_COMPONENT(COM_NAME, COM_TYPE)     Ptr<C##COM_NAME> COM_NAME() { return (C##COM_NAME*)m_component[(UINT)COMPONENT_TYPE::COM_TYPE].Get(); }

class GameObject : public Entity
{
public:
	GameObject();
	virtual ~GameObject();

public:
	void Begin();
	void Tick();
	void FinalTick();
	void Render();

public:
	void AddComponent(Ptr<Component> com);
	Ptr<Component> GetComponent(COMPONENT_TYPE type) { return m_component[UINT(type)]; }
	
	void AddChild(Ptr<GameObject> _Child) 
	{
		if (m_layerIdx != -1)
			_Child->m_layerIdx = m_layerIdx;
		m_vecChild.push_back(_Child); 
		_Child->m_parent = this;
	}

	Ptr<GameObject> GetParent() { return m_parent; }
	Ptr<GameObject> GetChild(int _idx) { return m_vecChild[_idx]; }
	const vector<Ptr<GameObject>> GetChild() { return m_vecChild; }

	bool IsDead() { return m_dead; }

	// 특정 컴포넌트를 다운캐스팅해서 바로 리턴
    GET_COMPONENT(Light2D, LIGHT2D);
	GET_COMPONENT(Transform, TRANSFORM);
	GET_COMPONENT(MeshRenderer, MESHRENDERER);
	GET_COMPONENT(BillboardRenderer, BILLBOARD_RENDER);
	GET_COMPONENT(Camera, CAMERA);
	GET_COMPONENT(Collider2D, COLLIDER2D);
	GET_COMPONENT(SpriteRenderer, SPRITE_RENDERER);
	GET_COMPONENT(TileRenderer, TILE_RENDERER);
	GET_COMPONENT(FlipbookRenderer, FLIPBOOK_RENDERER);

	Ptr<CTransform> GetTransform() { return (CTransform*)(m_component[(UINT)COMPONENT_TYPE::TRANSFORM].Get()); }
	Ptr<CMeshRenderer> GetMeshRenderer() { return (CMeshRenderer*)(m_component[(UINT)COMPONENT_TYPE::MESHRENDERER].Get()); }
	Ptr<CCamera> GetCamera() { return (CCamera*)(m_component[(UINT)COMPONENT_TYPE::CAMERA].Get()); }
	Ptr<CBillboardRenderer> GetBillboardRenderer() { return (CBillboardRenderer*)(m_component[(UINT)COMPONENT_TYPE::BILLBOARD_RENDER].Get()); }

	Ptr<CRenderComponent> GetRenderer() { return m_rendererCom; }

	void Destroy();

private:
	void RegisterLayer();

private:
	Ptr<Component> m_component[(UINT)COMPONENT_TYPE::END];
	vector<Ptr<CScript>> m_vecScripts;
	Ptr<CRenderComponent>   m_rendererCom;
	GameObject* m_parent;
	vector<Ptr<GameObject>> m_vecChild;

	// 자신의 소속 layer, -1일 경우 Scene에 속해있지 않음을 의미
	int m_layerIdx;		
	bool m_dead;

	friend class Layer;
	friend class TaskManager;

	// TEMPS
public:
	Dotween* m_dotween;
};

// obj가 해당 프레임에 IsDead인지 확인.
bool IsValid(Ptr<GameObject>& obj);

