#pragma once
#include "EditorUI.h"
#include "TransformUI.h"
#include "ComponentUI.h"
#include "AssetUI.h"

class Inspector : public EditorUI
{
public:
	Inspector();
	virtual ~Inspector();

public:
	virtual void Tick_UI() override;

public:
	void SetTargetObject(Ptr<GameObject> _obj);
	Ptr<GameObject> GetTargetObject() { return m_targetObj; }

	void SetTargetAsset(Ptr<Asset> _Asset);
	Ptr<Asset> GetTargetAsset() { return m_TargetAsset; }

private:
	void CreateChildUI();

private:
	Ptr<GameObject> m_targetObj;
	Ptr<ComponentUI> m_arrComUI[(UINT)COMPONENT_TYPE::END];

	Ptr<Asset> m_TargetAsset;
	Ptr<AssetUI> m_arrAssetUI[(UINT)ASSET_TYPE::END];
};

