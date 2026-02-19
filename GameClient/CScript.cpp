#include "pch.h"
#include "CScript.h"
#include "TaskManager.h"

CScript::CScript()
	: Component(COMPONENT_TYPE::SCRIPT)
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
