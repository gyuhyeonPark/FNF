#include "pch.h"
#include "GameObject.h"
#include "CTransform.h"
#include "TaskManager.h"
#include "SceneManager.h"
#include "Source/ScriptMgr.h"

GameObject::GameObject()
	:m_component{}, m_parent(nullptr), m_layerIdx(-1), m_dead(false), m_isActive(true)
{
}

GameObject::GameObject(const GameObject& _Origin)
	: Entity(_Origin)
	, m_component{}
	, m_parent(nullptr)
	, m_layerIdx(-1)
	, m_dead(false)
	, m_isActive(_Origin.m_isActive)
{
	// 원본 오브젝트와 동일한 세팅의 컴포넌트를 복사해서 나한테 넣어준다.
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _Origin.m_component[i])
			continue;

		AddComponent(_Origin.m_component[i]->Clone());
	}

	// 원본 오브젝트와 동일한 스크립트를 복사해서 나한테 넣어준다.
	for (const auto& Script : _Origin.m_vecScripts)
	{
		AddComponent(Script->Clone());
	}

	// 원본 오브젝트가 보유한 자식 오브젝트를 복사해서 나한테 넣어준다.
	for (const auto& Child : _Origin.m_vecChild)
	{
		AddChild(Child->Clone());
	}
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
		if (m_vecChild[i]->IsActive())
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

void GameObject::FinalTick_Editor()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_component[i])
			m_component[i]->FinalTick();
	}

	// 자식 오브젝트 FinalTick 호출
	// 만약 Dead 상태인 자식 오브젝트가 있으면, Vector 에서 제거한다.
	vector<Ptr<GameObject>>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		if ((*iter)->IsActive() == false)
			continue;

		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}
}

void GameObject::SaveToSceneFile(FILE* _File)
{
	// 이름
	SaveWString(_File, GetName());

	// 컴포넌트
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_component[i] == nullptr)
			continue;

		m_component[i]->SaveToSceneFile(_File);
	}

	// Script
	size_t ScriptCount = m_vecScripts.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (const auto& Script : m_vecScripts)
	{
		wstring ScriptName = ScriptMgr::GetScriptName(Script.Get());
		SaveWString(_File, ScriptName);

		Script->SaveToSceneFile(_File);
	}

}

void GameObject::LoadFromSceneFile(FILE* _File)
{
	// 이름
	SetName(LoadWString(_File));

	// 컴포넌트

	// Script
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName = LoadWString(_File);
		Ptr<CScript> pScript = ScriptMgr::GetScript(ScriptName);
		AddComponent(pScript.Get());

		pScript->LoadFromSceneFile(_File);
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
	if (m_rendererCom != nullptr && m_isActive)
	{
		GetTransform()->Binding();
		m_rendererCom->Render();
	}
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive() == false)
			continue;

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

void GameObject::AddChild(Ptr<GameObject> _Child)
{
	// 부모 오브젝트가 있는지 확인
	if (_Child->GetParent().Get())
	{
		// 기존 부모 오브젝트와 관계를 해제한다.
		_Child->DisconnectWithParent();
	}

	// 최상위 부모 오브젝트 였다면
	else
	{
		// 레벨 내부에 있던 오브젝트 라면
		if (_Child->m_layerIdx != -1)
		{
			// Layer 에서 최상위 부모로 가리키던 포인터를 제거
			_Child->DeregisterAsParent();
		}
	}

	m_vecChild.push_back(_Child);
	_Child->m_parent = this;

	if (_Child->m_layerIdx == -1)
	{
		_Child->m_layerIdx = m_layerIdx;
		
		// 부모가 될 오브젝트는 레벨 내부 소속인 경우 + 레벨이 Play 모드
		if (m_layerIdx != -1 && SceneManager::GetInstance()->GetSceneState() == SCENE_STATE::PLAY)
		{
			// Play 중인 씬 안에 있는 어떤 오브젝트의 자식으로서 씬에 합류했기 때문에,
			// Begin() 호출한다.
			_Child->Begin();
		}
	}
	
	// 부모 오브젝트가 씬 소속이면
	if (m_layerIdx != -1)
	{
		// 현재 씬에 변경이 발생했음을 알린다.
		SceneManager::GetInstance()->GetCurrentScene()->SetChanged();
	}
}

void GameObject::DisconnectWithParent()
{
	if (nullptr == m_parent)
		return;

	if (m_layerIdx != -1)
		SceneManager::GetInstance()->GetCurrentScene()->SetChanged();

	vector<Ptr<GameObject>>::iterator iter = m_parent->m_vecChild.begin();

	for (; iter != m_parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_parent->m_vecChild.erase(iter);
			m_parent = nullptr;
			return;
		}
	}

	assert(nullptr);
}

void GameObject::RegisterAsParent()
{
	if (m_layerIdx == -1)
		return;

	SceneManager::GetInstance()->GetCurrentScene()->GetLayer(m_layerIdx)->AddObject(this);
}

void GameObject::DeregisterAsParent()
{
	Ptr<AScene> pCurScene = SceneManager::GetInstance()->GetCurrentScene();
	Layer* pLayer = pCurScene->GetLayer(m_layerIdx);
	pLayer->DeregisterAsParent(this);
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

