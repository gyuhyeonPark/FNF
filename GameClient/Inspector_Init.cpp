#include "pch.h"
#include "Inspector.h"

#include "TransformUI.h"
#include "CameraUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "MeshRendererUI.h"
#include "SpriteRendererUI.h"
#include "FlipbookRendererUI.h"
#include "TileRendererUI.h"

#define ADD_COMPONENT_UI(ComponentType, type, Size) m_arrComUI[(UINT)ComponentType] = new type;\
													m_arrComUI[(UINT)ComponentType]->SetSizeAsChild(Size);\
													AddChildUI(m_arrComUI[(UINT)ComponentType].Get());

void Inspector::CreateChildUI()
{
	ADD_COMPONENT_UI(COMPONENT_TYPE::TRANSFORM,			TransformUI,		Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::COLLIDER2D,		Collider2DUI,		Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::CAMERA,			CameraUI,			Vec2(0.f, 300.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::LIGHT2D,			Light2DUI,			Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::MESHRENDERER,		MeshRendererUI,		Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::SPRITE_RENDERER,	SpriteRendererUI,	Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::FLIPBOOK_RENDERER,	FlipbookRendererUI,	Vec2(0.f, 200.f));
	ADD_COMPONENT_UI(COMPONENT_TYPE::TILE_RENDERER,		TileRendererUI,		Vec2(0.f, 200.f));
}