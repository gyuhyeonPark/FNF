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

private:
    wstring GetAssetName(ASSET_TYPE _Type, const wstring& _Name);

private:
    void File();
    void Scene();
    void View();
    void GameObjectMenu();
    void Asset();
};

