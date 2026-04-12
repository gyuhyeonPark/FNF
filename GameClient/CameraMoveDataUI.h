#pragma once
#include "ISongDataUI.h"
class CameraMoveDataUI : public ISongDataUI
{
public:
	CameraMoveDataUI();
	virtual ~CameraMoveDataUI();

public:
	virtual void Tick_UI() {};

public:
	virtual void Update() override;
	virtual void DrawOnWaveForm(int _idx) override;
	virtual void ClearList() override;


	virtual void Save(Document& doc) override;
	virtual void Load(Document& doc) override;

private:
	virtual void Record() override;
	virtual void Undo() override;
	virtual void Delete() override;
	virtual void DetectPicking() override;

private:
	std::list<int> eventTimings[2];
	pair<TapIter, std::list<int>*> m_pickEventInfo;
};

