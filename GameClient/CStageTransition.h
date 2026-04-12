#pragma once
#include "CScript.h"

STAGENUM stageArr[] =
{
	STAGENUM::LTR,
/*	STAGENUM::FRESH,
	STAGENUM::DADBATTLE,
	STAGENUM::SENPAI,*/
};

wstring stageNameArr[] =
{
	 L"LTR",
/*	 L"Fresh",
	 L"Dadbattle",
	 L"Senpai",*/
};
static int stageIdx = 0;

class CStageTransition : public CScript
{
public:
	CStageTransition(float duration = 2.f);
	CStageTransition(const CStageTransition& origin);
	virtual ~CStageTransition();

public:
	CLONE(CStageTransition);

	virtual void Tick() override;

	virtual void SaveToSceneFile(FILE* _File) override {}
	virtual void LoadFromSceneFile(FILE* _File) override {}

	void TriggerOn() { m_trigger = true; }

protected:
	bool m_trigger;
	bool m_isSceneCreated;
	float m_elapsed;
	float m_duration;
};

