#include "pch.h"
#include "Layer.h"
#include "GameObject.h"

Layer::Layer()
{
}

Layer::~Layer()
{
}

/// <summary>
/// 레이어에 게임 오브젝트를 추가하고 해당 오브젝트와 모든 자손의 레이어 인덱스(m_layerIdx)를 현재 레이어 인덱스로 설정합니다.
/// </summary>
/// <param name="obj">
/// 추가할 게임 오브젝트에 대한 스마트 포인터(Ptr<GameObject>). m_parentVec에 저장되며, 오브젝트와 그 하위 자식들의 m_layerIdx가 현재 레이어 인덱스로 업데이트됩니다.</param>
void Layer::AddObject(Ptr<GameObject> obj)
{
	m_parentVec.push_back(obj);
	obj->m_layerIdx = m_layerIdx;

	list<GameObject*> queue;

	queue.push_back(obj.Get());

	while (!queue.empty())
	{
		GameObject* pObj = queue.front();
		queue.pop_front();
		pObj->m_layerIdx = m_layerIdx;

		for (int i = 0; i < pObj->m_vecChild.size(); i++)
		{
			queue.push_back(pObj->m_vecChild[i].Get());
		}
	}
}

void Layer::Begin()
{
	for (size_t i = 0; i < m_parentVec.size(); i++)
	{
		m_parentVec[i]->Begin();
	}
}

void Layer::Tick()
{
	for (size_t i = 0; i < m_parentVec.size(); i++)
	{
		m_parentVec[i]->Tick();
	}
}

void Layer::FinalTick()
{
	vector<Ptr<GameObject>>::iterator iter = m_parentVec.begin();

	for (; iter != m_parentVec.end();)
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_parentVec.erase(iter);
		else
			++iter;
	}
}

void Layer::Render()
{
	for (size_t i = 0; i < m_parentVec.size(); i++)
	{
		m_parentVec[i]->Render();
	}
}
