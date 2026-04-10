#pragma once
#include "ComponentUI.h"
class SpriteRendererUI :
    public ComponentUI
{
public:
    virtual void Tick_UI() override;

public:
    SpriteRendererUI();
    virtual ~SpriteRendererUI();
};

