#pragma once
#include "EditorUI.h"
class ListUI :
    public EditorUI
{
public:
    ListUI();
    virtual ~ListUI();

public:
    virtual void Tick_UI() override;
    virtual void Activate() override;
    virtual void DeActivate() override;

public:
    void AddString(const string& str) { m_vecList.push_back(str); }
    void AddString(const wstring& wStr) { m_vecList.push_back(string(wStr.begin(), wStr.end())); }
    void AddString(const vector<string>& vecStr) { m_vecList.insert(m_vecList.end(), vecStr.begin(),vecStr.end()); }
    void AddString(const vector<wstring>& vecWStr)
    {
        for (size_t i = 0; i < vecWStr.size(); ++i)
        {
            AddString(vecWStr[i]);
        }
    }

    void AddDelegate(EditorUI* _inst, DELEGATE_1 _memFunc) { m_inst = _inst; m_memFunc = _memFunc; }

    const string& GetSelectedString() { return m_selectedString; }

private:
    vector<string> m_vecList;
    int m_selectedIdx;
    string m_selectedString;

    EditorUI* m_inst;
    DELEGATE_1 m_memFunc;
};

