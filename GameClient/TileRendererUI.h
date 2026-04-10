#pragma once
#include "ComponentUI.h"
class TileRendererUI :
    public ComponentUI
{
public:
    virtual void Tick_UI() override;

public:
    TileRendererUI();
    virtual ~TileRendererUI();
};

