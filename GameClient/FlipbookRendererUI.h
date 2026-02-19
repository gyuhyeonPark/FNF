#pragma once
#include "ComponentUI.h"
class FlipbookRendererUI :
    public ComponentUI
{
public:
    virtual void Tick_UI() override;

public:
    FlipbookRendererUI();
    virtual ~FlipbookRendererUI();
};

