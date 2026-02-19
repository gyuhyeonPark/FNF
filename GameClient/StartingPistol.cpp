#include "pch.h"
#include "StartingPistol.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "GameManager.h"
#include "EventManager.h"

StartingPistol::StartingPistol()
	: m_elapsed(0.f), m_duration(0.f), m_funcIdx(0), m_mtrlTexIdx(0)
{
}

StartingPistol::~StartingPistol()
{
}

void StartingPistol::Begin()
{
	// Ready-Set-Go 오브젝트 생성
	// 하나의 Material에 세 개의 텍스처를 붙여보자

	assert(m_duration > 0.f, "SetBPM을 통해 노래의 BPM을 설정해주세요");
	GetOwner()->GetMeshRenderer()->GetMtrl()->SetScalar(FLOAT_1, m_duration);
	GetOwner()->GetMeshRenderer()->GetMtrl()->SetScalar(INT_0, m_mtrlTexIdx);

	m_funcArr[0] = []()
		{
			SOUNDMANAGER->PlayRSG(SoundType::THREE);
		};
	m_funcArr[1] = [this]()
		{
			SOUNDMANAGER->PlayRSG(SoundType::TWO);
			GetOwner()->GetMeshRenderer()->GetMtrl()->SetScalar(INT_0, ++m_mtrlTexIdx);
		};
	m_funcArr[2] = [this]()
		{
			SOUNDMANAGER->PlayRSG(SoundType::ONE);
			GetOwner()->GetMeshRenderer()->GetMtrl()->SetScalar(INT_0, ++m_mtrlTexIdx);
		};
	m_funcArr[3] = [this]()
		{
			SOUNDMANAGER->PlayRSG(SoundType::GO);
			GetOwner()->GetMeshRenderer()->GetMtrl()->SetScalar(INT_0, ++m_mtrlTexIdx);
		};
	m_funcArr[4] = [this]()
		{
			SOUNDMANAGER->PlayStage(STAGENUM::BOPEEBO);
			GetOwner()->GetMeshRenderer()->GetMtrl()->SetScalar(INT_0, ++m_mtrlTexIdx);
			m_gm->MusicStart();
			m_em->Start();
		};
}

void StartingPistol::Tick()
{
	if (m_funcIdx < 5)
	{
		m_elapsed += DT;

		// shader에 texture alpha값을 변환시키는 값 전달
		GetOwner()->GetMeshRenderer()->GetMtrl()->SetScalar(FLOAT_0, m_elapsed);
		GetOwner()->GetMeshRenderer()->GetMtrl()->SetScalar(FLOAT_1, m_duration);
		
		if (m_elapsed >= m_duration)
		{
			// shader에 tex를 변환시키는 값 전달
			m_elapsed -= m_duration;
			m_funcArr[m_funcIdx]();
			++m_funcIdx;
		}
	}
}
