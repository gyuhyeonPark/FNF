#pragma once
#include "CRenderComponent.h"
#include "assets.h"

class CMeshRenderer : public CRenderComponent
{
public:
	CMeshRenderer();
	virtual ~CMeshRenderer();

public:
	virtual void FinalTick() override;
	virtual void Render() override;
};

