#pragma once
#include "EditorUI.h"

class ComponentUI : public EditorUI
{
public:
	ComponentUI(COMPONENT_TYPE type, const string& name);
	virtual ~ComponentUI();

public:
	void SetTarget(Ptr<GameObject> obj);
	Ptr<GameObject> GetTarget() { return m_target; }

protected:
	void OutputTitle(const string& _title);

private:
	Ptr<GameObject> m_target;
	const COMPONENT_TYPE m_comType;
};

