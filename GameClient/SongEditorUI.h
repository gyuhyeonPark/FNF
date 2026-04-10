#pragma once
#include "EditorUI.h"

class SongEditorUI : public EditorUI
{
public:
	SongEditorUI();
	virtual ~SongEditorUI();

public:
	virtual void Tick_UI() override;
};

