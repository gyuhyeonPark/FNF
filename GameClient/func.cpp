#include "pch.h"
#include "func.h"
#include "RenderManager.h"
#include "TaskManager.h"
#include "GameObject.h"

void CreateObject(GameObject* object, int layeridx)
{
	TaskInfo info = {};

	info.type = TASK_TYPE::CREATE_OBJECT;
	info.param_0 = (DWORD_PTR)object;
	info.param_1 = layeridx;

	TaskManager::GetInstance()->AddTask(info);
}

void DrawDebugRect(Vec3 pos, Vec3 scale, Vec3 rot, Vec4 color, float duration, bool depthTest)
{
	DbgInfo info = {};

	info.shape = DBG_SHAPE::RECT;
	info.pos = pos;
	info.scale = scale;
	info.rotation = rot;
	info.matWorld = XMMatrixIdentity();
	info.color = color;
	info.elapsed = 0.f;
	info.duration = duration;
	info.depthTest = depthTest;
	RenderManager::GetInstance()->AddDebugInfo(info);
}

void DrawDebugRect(Matrix matWorld, Vec4 color, float duration, bool depthTest)
{
	DbgInfo info = {};

	info.shape = DBG_SHAPE::RECT;

	info.matWorld = matWorld;
	info.color = color;
	info.elapsed = 0.f;
	info.duration = duration;
	info.depthTest = depthTest;
	RenderManager::GetInstance()->AddDebugInfo(info);
}

void DrawDebugCircle(Vec3 pos, float radius, Vec4 color, float duration, bool depthTest)
{
	DbgInfo info = {};

	info.shape = DBG_SHAPE::CIRCLE;
	info.pos = pos;
	info.scale = Vec3(radius * 2.f, radius * 2.f, 0.f);
	info.rotation = Vec3(0.f, 0.f, 0.f);
	info.color = color;
	info.elapsed = 0.f;
	info.duration = duration;
	info.depthTest = depthTest;
	RenderManager::GetInstance()->AddDebugInfo(info);
}

bool IsValid(Ptr<GameObject>& obj)
{
	if (obj == nullptr || obj->IsDead())
	{
		obj = nullptr;
		return false;
	}

	return true;
}