#pragma once
#include "NoteReceptor.h"

class OpponentNoteReceptor : public NoteReceptor
{
public:
	OpponentNoteReceptor();
	virtual ~OpponentNoteReceptor();

public:
	virtual void Tick() override;

private:
	virtual void ActivateTapEvent() override;
	virtual void ActivatePressedEvent() override;
	virtual void ActivateReleasedEvent() override;
};

