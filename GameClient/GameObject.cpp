#include "pch.h"
#include "GameObject.h"
#include "CTransform.h"
#include "CAtlasAnimator.h"
#include "TaskManager.h"
#include "SceneManager.h"

GameObject::GameObject()
	:m_component{}, m_parent(nullptr), m_layerIdx(-1), m_dead(false)
{
}

GameObject::~GameObject()
{
}

void GameObject::Begin()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		if (m_component[i] != nullptr)
			m_component[i]->Begin();
	}

	for (UINT i = 0; i < m_vecScripts.size(); i++)
	{
		m_vecScripts[i]->Begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}

void GameObject::Tick()
{
	for (UINT i = 0; i < m_vecScripts.size(); i++)
	{
		m_vecScripts[i]->Tick();
	}
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void GameObject::FinalTick()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		if (m_component[i] != nullptr)
			m_component[i]->FinalTick();
	}

	// 자신이 소속된 Layer에 자기 자신을 등록
	RegisterLayer();

	// 자식 오브젝트 FinalTick 호출
	// 만약 Dead 상태인 자식이 있다면 vector에서 제거한다.
	vector<Ptr<GameObject>>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end();)
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}
}

void GameObject::RegisterLayer()
{
	Ptr<AScene> curScene = SceneManager::GetInstance()->GetCurrentScene();
	Layer* layer = curScene->GetLayer(m_layerIdx);
	layer->RegisterObject(this);
}

void GameObject::Render()
{
	if (m_rendererCom != nullptr)
	{
		GetTransform()->Binding();
		m_rendererCom->Render();
	}
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Render();
	}
}

void GameObject::AddComponent(Ptr<Component> com)
{
	// MeshRenderer 컴포넌트를 하나만 가질 수 있도록 강제
	if (dynamic_cast<CRenderComponent*>(com.Get()))
	{
		assert(m_rendererCom == nullptr);
		m_rendererCom = (CRenderComponent*)com.Get();
	}

	if (com->m_type == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScripts.push_back((CScript*)com.Get());
	}
	else
	{
		assert(m_component[UINT(com->GetType())] == nullptr);
		m_component[UINT(com->GetType())] = com;
	}
	com->m_ownerObject = this;
	com->Init();
}

void GameObject::Destroy()
{
	if (!m_dead)
	{
		TaskInfo info = {};
		info.type = TASK_TYPE::DESTROY_OBJECT;
		info.param_0 = (DWORD_PTR)this;
		TaskManager::GetInstance()->AddTask(info);
	}
}

