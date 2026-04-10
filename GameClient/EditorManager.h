#pragma once
#include "SingletonClass.h"
#include "EditorUI.h"

class EditorManager : public SingletonClass<EditorManager>
{
	SINGLE(EditorManager);
public:
	virtual ~EditorManager();

	void Init(HWND _hwnd);
	void Progress();

public:
	void AddUI(const string& _UIName, Ptr<EditorUI> _UI);
	Ptr<EditorUI> FindUI(const string& _UIName);

	bool IsShowDemo() { return m_ShowDemo; }
	void ShowDemo(bool _Show) { m_ShowDemo = _Show; }

	void RegisterFocusedUI(Ptr<EditorUI> _ui) { m_focusedUI = _ui; }

private:
	void Tick();
	void Render();

private:
	void CreateEditorUI();
	void CreateEditorObject();

private:
	map<string, Ptr<EditorUI>> m_uiMap;
	bool m_ShowDemo;
	Ptr<EditorUI> m_focusedUI;

	vector<Ptr<GameObject>> m_editorObject;
};

