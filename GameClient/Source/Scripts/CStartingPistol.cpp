#include "pch.h"
#include "CStartingPistol.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "CGameManager.h"
#include "CEventManager.h"
#include "SceneManager.h"
#include "SongManager.h"

CStartingPistol::CStartingPistol()
	: CScript(SCRIPT_TYPE::STARTINGPISTOL),
	m_elapsed(0.f), m_duration(0.f), m_funcIdx(0), m_mtrlTexIdx(0)
{
}

CStartingPistol::CStartingPistol(const CStartingPistol& _Origin)
	: CScript(_Origin), m_elapsed(0.f), m_duration(_Origin.m_duration), m_funcIdx(0), m_mtrlTexIdx(0)
{
}

CStartingPistol::~CStartingPistol()
{
}

void CStartingPistol::Begin()
{
	// Ready-Set-Go 오브젝트 생성
	// 하나의 Material에 세 개의 텍스처를 붙여보자

	m_gm = SceneManager::GetInstance()->FindObjectByName(L"GM")->GetScript<CGameManager>().Get();
	m_em = SceneManager::GetInstance()->FindObjectByName(L"EM")->GetScript<CEventManager>().Get();

	//Ptr<AMaterial> mtrl = LOAD(AMaterial, L"Material\\RSG.mtrl");

	assert(m_duration > 0.f, "SetBPM을 통해 노래의 BPM을 설정해주세요");
	GetOwner()->GetMeshRenderer()->GetMaterial()->SetScalar(FLOAT_1, m_duration);
	GetOwner()->GetMeshRenderer()->GetMaterial()->SetScalar(INT_0, m_mtrlTexIdx);

	SongManager::GetInstance()->ChangeSong(SongManager::GetInstance()->GetCurrentStageNum());

	m_funcArr[0] = []()
		{
			SOUNDMANAGER->PlaySFX(SoundType::THREE);
		};
	m_funcArr[1] = [this]()
		{
			SOUNDMANAGER->PlaySFX(SoundType::TWO);
			GetOwner()->GetMeshRenderer()->GetMaterial()->SetScalar(INT_0, ++m_mtrlTexIdx);
		};
	m_funcArr[2] = [this]()
		{
			SOUNDMANAGER->PlaySFX(SoundType::ONE);
			GetOwner()->GetMeshRenderer()->GetMaterial()->SetScalar(INT_0, ++m_mtrlTexIdx);
		};
	m_funcArr[3] = [this]()
		{
			SOUNDMANAGER->PlaySFX(SoundType::GO);
			GetOwner()->GetMeshRenderer()->GetMaterial()->SetScalar(INT_0, ++m_mtrlTexIdx);
		};
	m_funcArr[4] = [this]()
		{
			SOUNDMANAGER->ChangeStage(SONGNUM);
			SOUNDMANAGER->MuteSong(false);
			GetOwner()->GetMeshRenderer()->GetMaterial()->SetScalar(INT_0, ++m_mtrlTexIdx);
			m_gm->MusicStart();
			m_em->Start();

			Destroy();
		};

	//MeshRenderer()->SetMaterial(mtrl);

}

void CStartingPistol::Tick()
{
	if (m_funcIdx < 5)
	{
		m_elapsed += DT;

		// shader에 texture alpha값을 변환시키는 값 전달
		GetOwner()->GetMeshRenderer()->GetMaterial()->SetScalar(FLOAT_0, m_elapsed);
		GetOwner()->GetMeshRenderer()->GetMaterial()->SetScalar(FLOAT_1, m_duration);
		
		if (m_elapsed >= m_duration)
		{
			// shader에 tex를 변환시키는 값 전달
			m_elapsed -= m_duration;
			m_funcArr[m_funcIdx]();
			++m_funcIdx;
		}
	}
}

void CStartingPistol::SaveToSceneFile(FILE* _File)
{
}

void CStartingPistol::LoadFromSceneFile(FILE* _File)
{
}
