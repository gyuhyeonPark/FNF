#pragma once
#include "AssetUI.h"
class SceneUI :
    public AssetUI
{
public:
    virtual void Tick_UI() override;

public:
    SceneUI();
    virtual ~SceneUI();
};

