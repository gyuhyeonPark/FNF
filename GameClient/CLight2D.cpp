#include "pch.h"
#include "CLight2D.h"
#include "RenderManager.h"

CLight2D::CLight2D()
	: Component(COMPONENT_TYPE::LIGHT2D)
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::FinalTick()
{
	m_info.WorldPos = Transform()->GetRelativePosition();
	m_info.LightDir = Transform()->GetDir(DIR::RIGHT);
	RenderManager::GetInstance()->RegisterLight2D(this);
}
