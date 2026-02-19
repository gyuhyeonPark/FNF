#pragma once
#include "EditorUI.h"
#include "TransformUI.h"
#include "ComponentUI.h"

class Inspector : public EditorUI
{
public:
	Inspector();
	virtual ~Inspector();

public:
	virtual void Tick_UI() override;

public:
	void SetTargetObject(Ptr<GameObject> _obj);
	void CreateChildUI();
private:
	Ptr<GameObject> m_targetObj;
	Ptr<ComponentUI> m_arrComUI[(UINT)COMPONENT_TYPE::END];
};

