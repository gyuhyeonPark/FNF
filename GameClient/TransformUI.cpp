#include "pch.h"
#include "TransformUI.h"
#include "GameObject.h"

TransformUI::TransformUI()
	: ComponentUI(COMPONENT_TYPE::TRANSFORM, "TransformUI")
{
}

TransformUI::~TransformUI()
{
}

void TransformUI::Tick_UI()
{
	OutputTitle("Transform");
	Vec3 vPos = GetTarget()->Transform()->GetRelativePosition();
	Vec3 vScale = GetTarget()->Transform()->GetRelativeScale();
	Vec3 vRot = GetTarget()->Transform()->GetRelativeRotation();
	
	ImGui::Text("Position");
	ImGui::SameLine(100);
	if (ImGui::DragFloat3("##POSITION", vPos))
		GetTarget()->Transform()->SetRelativePosition(vPos);

	ImGui::Text("Scale");
	ImGui::SameLine(100); 
	if (ImGui::DragFloat3("##SCALE", vScale))
		GetTarget()->Transform()->SetRelativeScale(vScale);

	
	Vec3 vDegree = vRot * 180 / XM_PI;
	ImGui::Text("Rotation");
	ImGui::SameLine(100);
	if (ImGui::DragFloat3("##ROTATION", vDegree))
	{
		vRot = vDegree * (XM_PI / 180.f);
		GetTarget()->Transform()->SetRelativeRotation(vRot);
	}
}
