#pragma once
#define GET_OTHER_COMPONENT(COM_NAME) class C##COM_NAME* COM_NAME();

class GameObject;

class Component : public Entity
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();

public:
	COMPONENT_TYPE GetType() { return m_type; }
	GameObject* GetOwner() { return m_ownerObject; }
    GET_OTHER_COMPONENT(Light2D);
	GET_OTHER_COMPONENT(Transform);
	GET_OTHER_COMPONENT(MeshRenderer);
	GET_OTHER_COMPONENT(BillboardRenderer);
	GET_OTHER_COMPONENT(Camera);
	GET_OTHER_COMPONENT(SpriteRenderer);
	GET_OTHER_COMPONENT(Collider2D);
	GET_OTHER_COMPONENT(FlipbookRenderer);
	GET_OTHER_COMPONENT(TileRenderer);

	virtual void Init() {}
	virtual void Begin() {}
	virtual void FinalTick() = 0;

private:
	const COMPONENT_TYPE m_type;
	GameObject* m_ownerObject;		// 주의! Ptr로 가리키지 않는다.

	friend class GameObject;
};

