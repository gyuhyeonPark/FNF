#pragma once
#include "SingletonClass.h"
class TaskManager : public SingletonClass<TaskManager>
{
	SINGLE(TaskManager);

public:
	virtual ~TaskManager();
	void Progress();

	void AddTask(TaskInfo info) { m_vecTask.push_back(info); }

private:
	vector<TaskInfo> m_vecTask;
	vector<Ptr<GameObject>> m_garbage;
};

