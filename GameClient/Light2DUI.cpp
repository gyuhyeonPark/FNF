#include "pch.h"
#include "Light2DUI.h"
#include "GameObject.h"

Light2DUI::Light2DUI()
	: ComponentUI(COMPONENT_TYPE::LIGHT2D, "Light2D")
{
}

Light2DUI::~Light2DUI()
{
}

void Light2DUI::Tick_UI()
{
	OutputTitle("Light2D");

	Light2DInfo info = GetTarget()->Light2D()->GetInfo();

	const char* types[] = { "Directional", "Point", "Spot" };
	static int type_current = (int)info.Type;
	if (ImGui::Combo("LIGHT TYPE", &type_current, types, IM_COUNTOF(types)))
	{
		info.Type = LIGHT_TYPE(type_current);
		GetTarget()->Light2D()->SetLightType(info.Type);
	}

	if (ImGui::ColorEdit3("Light Color", info.Color))
		GetTarget()->Light2D()->SetLightColor(info.Color);

/*	Vec3 lightRot = GetTarget()->Transform()->GetRelativeRotation();
	Vec3 vDegree = lightRot * 180 / XM_PI;

	if (ImGui::DragFloat3("Direction", lightRot))
	{
		lightRot = vDegree * (XM_PI / 180.f);
		GetTarget()->Light2D()->SetLightDir(lightRot);
	}*/
		
	if (ImGui::DragFloat3("Ambient", info.Ambient))
		GetTarget()->Light2D()->SetLightDir(info.Ambient);

	if (ImGui::DragFloat("Radius", &info.Radius))
		GetTarget()->Light2D()->SetRadius(info.Radius);

	if (ImGui::DragFloat("Angle", &info.Angle))
		GetTarget()->Light2D()->SetAngle(info.Angle);
}
