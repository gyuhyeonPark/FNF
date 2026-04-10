#pragma once
#include "Component.h"
#include "ASprite.h"
#include "CRenderComponent.h"

class CSpriteRenderer : public CRenderComponent
{
public:
	CSpriteRenderer();
	virtual ~CSpriteRenderer();

	virtual void FinalTick() override;
	virtual void Render() override;
	virtual void CreateMaterial() override;

public:
	GET_SET(Ptr<ASprite>, Sprite);

	CLONE(CSpriteRenderer);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	Ptr<ASprite> m_Sprite;
};

