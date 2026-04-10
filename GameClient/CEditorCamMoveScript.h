#pragma once
#include "CScript.h"

class CEditorCamMoveScript :
    public CScript
{
private:
    float   m_Speed;

public:
    virtual void Tick() override;

private:
    void MovePerspective();
    void MoveOrthographic();


    CLONE(CEditorCamMoveScript);

    virtual void SaveToSceneFile(FILE* _File) override;
    virtual void LoadFromSceneFile(FILE* _File) override;

public:
    CEditorCamMoveScript();
    virtual ~CEditorCamMoveScript();
};

