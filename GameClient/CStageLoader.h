#pragma once
#include "CScript.h"
class CStageLoader : public CScript
{
public:
	CStageLoader(STAGENUM nextStageNum, wstring nextSceneName);
	virtual ~CStageLoader();

public:
	CLONE(CStageLoader);

	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToSceneFile(FILE* _File) override {}
	virtual void LoadFromSceneFile(FILE* _File) override {}

private:
	STAGENUM m_nextStageNum;
	wstring m_nextSceneName;

	float m_elapsed;
	bool m_isCreated = false;
};

