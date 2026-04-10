#include "pch.h"
#include "ContentUI.h"

#include "TreeUI.h"
#include "AssetManager.h"

ContentUI::ContentUI()
	: EditorUI("Content")
{
	m_Tree = new TreeUI;
	m_Tree->SetSaperator(false);
	AddChildUI(m_Tree.Get());

	// Asset 내용을 트리에 반영
	Renew();
}

ContentUI::~ContentUI()
{
}


void ContentUI::Tick_UI()
{
	if (AssetManager::GetInstance()->IsChanged())
		Renew();
}

void ContentUI::Renew()
{
	m_Tree->Clear();

	// 애셋 종류별로 Tree에 추가하기
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		// 애셋의 이름에 해당하는 노드를 추가 (enum -> 문자열)
		Ptr<TreeNode> pNode = m_Tree->AddItem(nullptr, ToString((ASSET_TYPE)i));
		pNode->SetFramed(true);

		// 해당 에셋 모든 이름을 받아와서 하위 자식으로 추가
		vector<wstring> vecNames;
		AssetManager::GetInstance()->GetAssetNames((ASSET_TYPE)i, vecNames);

		for (const auto& str : vecNames)
		{
			Ptr<Asset> pAsset = AssetManager::GetInstance()->FindAsset((ASSET_TYPE)i, str);
			m_Tree->AddItem(pNode, string(str.begin(), str.end()), (DWORD_PTR)pAsset.Get());
		}
	}
}
