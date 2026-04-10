#include "pch.h"
#include "CMeshRenderer.h"

CMeshRenderer::CMeshRenderer()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDERER)
{
}

CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::SaveToSceneFile(FILE* _File)
{
}

void CMeshRenderer::LoadFromSceneFile(FILE* _File)
{
}

void CMeshRenderer::FinalTick()
{
}

void CMeshRenderer::Render()
{
	if (GetMesh() == nullptr || GetMaterial() == nullptr)
		return;

	GetMaterial()->Binding();
	GetMesh()->Render();

	GetMaterial()->Clear();
}
