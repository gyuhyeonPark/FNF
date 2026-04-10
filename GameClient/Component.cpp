#include "pch.h"
#include "Component.h"
#include "components.h"
#include "GameObject.h"

#define GET_OTHER_COMPONENT_BODY(COM_NAME) C##COM_NAME* Component::COM_NAME() { return GetOwner()->COM_NAME().Get(); }

Component::Component(COMPONENT_TYPE type)
	: m_ownerObject(nullptr), m_type(type)
{
}

Component::Component(const Component& _Origin)
	: Entity(_Origin)
	, m_ownerObject(nullptr)
	, m_type(_Origin.m_type)
{
}

Component::~Component()
{
}

GET_OTHER_COMPONENT_BODY(Transform);
GET_OTHER_COMPONENT_BODY(MeshRenderer);
GET_OTHER_COMPONENT_BODY(BillboardRenderer);
GET_OTHER_COMPONENT_BODY(Camera);
GET_OTHER_COMPONENT_BODY(SpriteRenderer);
GET_OTHER_COMPONENT_BODY(Collider2D);
GET_OTHER_COMPONENT_BODY(FlipbookRenderer);
GET_OTHER_COMPONENT_BODY(TileRenderer);

GET_OTHER_COMPONENT_BODY(Light2D);
GET_OTHER_COMPONENT_BODY(AtlasAnimator);
