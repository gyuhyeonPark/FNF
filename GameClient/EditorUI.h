#pragma once
#include "Entity.h"
#include "imgui.h"

class EditorUI : public Entity
{
public:
	EditorUI(const string& uiName);
	virtual ~EditorUI();

public:
	virtual void Tick();
	virtual void Tick_UI() = 0;
	virtual void Activate() {}
	virtual void DeActivate() {}

public:
	bool IsActive() { return m_active; }
	void SetActive(bool _Active)
	{
		if (m_active == _Active)
			return;

		m_active = _Active;
		
		if (m_active)
			Activate();
		else
			DeActivate();
	}

	void AddChildUI(Ptr<EditorUI> _child)
	{
		_child->m_parent = this;
		m_childUI.push_back(_child);
	}

	Ptr<EditorUI> GetParentUI() { return m_parent; }

	GET_SET(Vec2, SizeAsChild);
	GET_SET(string, UIName);

	void SetModal(bool _Modal) { m_IsModal = _Modal; }
	void SetSaperator(bool _set) { m_Separator = _set; }

protected:
	void SetUIKey(const string& _Key) { m_UIKey = _Key; }

private:

	void CheckFocus();

private:
	string m_UIName;
	string m_UIKey;

	bool m_IsModal;
	bool m_Separator;
	bool m_active;
	
	EditorUI* m_parent;
	vector<Ptr<EditorUI>> m_childUI;

	Vec2 m_SizeAsChild;
};

typedef void(EditorUI::* DELEGATE_0)(void);
typedef void(EditorUI::* DELEGATE_1)(DWORD_PTR);
typedef void(EditorUI::* DELEGATE_2)(DWORD_PTR, DWORD_PTR);