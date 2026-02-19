#pragma once
#include "EditorUI.h"
class Menu :
    public EditorUI
{
public:
    Menu();
    virtual ~Menu();

public:
    virtual void Tick() override;
    virtual void Tick_UI() override;
};

