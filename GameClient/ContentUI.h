#pragma once
#include "EditorUI.h"

class TreeUI;

class ContentUI :
    public EditorUI
{
public:
    ContentUI();
    virtual ~ContentUI();

public:
    virtual void Tick_UI() override;

public:
    void Renew();

private:
    Ptr<TreeUI> m_Tree;
};

