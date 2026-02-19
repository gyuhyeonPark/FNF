#pragma once
#include "ComponentUI.h"
class MeshRendererUI :
    public ComponentUI
{
public:
    virtual void Tick_UI() override;

private:
    void SelectMesh(DWORD_PTR _ListUI);

public:
    MeshRendererUI();
    virtual ~MeshRendererUI();
};

