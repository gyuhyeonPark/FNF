#pragma once
#include "CRenderComponent.h"
#include "assets.h"

class CMeshRenderer : public CRenderComponent
{
public:
	CMeshRenderer();
	virtual ~CMeshRenderer();

public:
	CLONE(CMeshRenderer);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

public:
	virtual void FinalTick() override;
	virtual void Render() override;
};

