#pragma once
#include "CStageTransition.h"

class TitleScript : public CStageTransition
{
public:
	TitleScript();
	virtual ~TitleScript();

public:
	CLONE(TitleScript);

	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToSceneFile(FILE* _File) override {}
	virtual void LoadFromSceneFile(FILE* _File) override {}
};

