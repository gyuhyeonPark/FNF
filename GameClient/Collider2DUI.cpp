#include "pch.h"
#include "Collider2DUI.h"
#include "GameObject.h"

Collider2DUI::Collider2DUI()
	: ComponentUI(COMPONENT_TYPE::COLLIDER2D, "Collider2D")
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Tick_UI()
{
	OutputTitle("Collider2D");

	Vec2 vScale = GetTarget()->Collider2D()->GetScale();
	Vec2 vOffset = GetTarget()->Collider2D()->GetOffset();

	// 오프셋과 스케일 조정
	if (ImGui::DragFloat2("SCALE", vScale, 0.1f))
		GetTarget()->Collider2D()->SetScale(vScale);
	if (ImGui::DragFloat2("OFFSET", vOffset, 0.1f))
		GetTarget()->Collider2D()->SetOffset(vOffset);
}
