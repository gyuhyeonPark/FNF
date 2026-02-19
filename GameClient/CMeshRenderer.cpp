#include "pch.h"
#include "CMeshRenderer.h"

CMeshRenderer::CMeshRenderer()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDERER)
{
}

CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::FinalTick()
{
}

void CMeshRenderer::Render()
{
	if (GetMesh() == nullptr || GetMtrl() == nullptr)
		return;

	GetMtrl()->Binding();
	GetMesh()->Render();

	GetMtrl()->Clear();
}
