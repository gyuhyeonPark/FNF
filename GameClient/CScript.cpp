#include "pch.h"
#include "CScript.h"
#include "TaskManager.h"

CScript::CScript(int _ScriptType)
	: Component(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_ScriptType)
{
}

CScript::CScript(const CScript& _Origin)
	: Component(_Origin), m_ScriptType(_Origin.m_ScriptType)
{
}

CScript::~CScript()
{
}

void CScript::Destroy()
{
	if (!GetOwner()->IsDead())
	{
		TaskInfo info = {};
		info.type = TASK_TYPE::DESTROY_OBJECT;
		info.param_0 = (DWORD_PTR)GetOwner();
		TaskManager::GetInstance()->AddTask(info);
	}
}
