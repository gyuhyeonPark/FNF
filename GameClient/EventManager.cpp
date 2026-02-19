#include "pch.h"
#include "EventManager.h"
#include "CPlayerController.h"
#include "COpponentController.h"
#include "CCamera.h"

EventManager::EventManager()
	: m_isStarted(false), m_elapsed(0.f), m_camEventIdx(0), m_animEventIdx(0)
{
}

EventManager::~EventManager()
{
}

void EventManager::Begin()
{
	// Parser 통해 정보 가져오기
	Parser::GetInstance()->LoadEventData(L"bopeebo", this);

	Vec3 camPos = m_cam->Transform()->GetRelativePosition();

	Vec3 playerPos = m_player->Transform()->GetRelativePosition();
	
	Vec3 dir = playerPos - camPos;
	dir.Normalize();
	dir = Vec3(dir.x, dir.y, 0.f);

	m_playerCamPos = camPos + dir * 80.f;

	Vec3 opponentPos = m_opponent->Transform()->GetRelativePosition();

	dir = opponentPos - camPos;
	dir.Normalize();
	dir = Vec3(dir.x, dir.y, 0.f);

	m_opponentCamPos = camPos + dir * 80.f;
}

void EventManager::Tick()
{
	if (m_isStarted)
	{
		m_elapsed += DT;

		if (m_camEventIdx < m_camEventVec.size() &&  m_elapsed >= m_camEventVec[m_camEventIdx].timing * 0.001f)
		{
			Vec3 pos = {};

			// 카메라 명령
			if (m_camEventVec[m_camEventIdx].value == 1)
				pos = m_opponentCamPos;
			else
				pos = m_playerCamPos;

			// Camera의 Dotween에게 지시하기
			m_cam->GetOwner()->m_dotween->DOMove(pos, 2.f)->SetEase(EaseOutExpo);
			++m_camEventIdx;
		}


		if (m_animEventIdx < m_animEventVec.size() && m_elapsed >= m_animEventVec[m_animEventIdx] * 0.001f)
		{
			// 애니메이션 Play
			m_player->PlayHEY();
			++m_animEventIdx;
		}
	}
		
}
