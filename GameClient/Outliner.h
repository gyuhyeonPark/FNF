#pragma once
#include "EditorUI.h"

struct TreeNode;
class TreeUI;
class GameObject;

class Outliner : public EditorUI
{
public:
	Outliner();
	virtual ~Outliner();

public:
	virtual void Tick_UI() override;

	// Tree 갱신, 현재 레벨의 최신 상태를 Tree 에 표시
	void Renew();

private:
	void AddGameObject(Ptr<TreeNode> _ParentNode, Ptr<GameObject> _Object);
	void SelectGameObject(DWORD_PTR _Object);

	void AddChild(DWORD_PTR _Src, DWORD_PTR _Dest);

private:
	Ptr<TreeUI> m_Tree;
};

