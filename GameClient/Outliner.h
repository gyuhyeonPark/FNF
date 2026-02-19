#pragma once
#include "EditorUI.h"
class Outliner : public EditorUI
{
public:
	Outliner();
	virtual ~Outliner();

public:
	virtual void Tick_UI() override;
};

