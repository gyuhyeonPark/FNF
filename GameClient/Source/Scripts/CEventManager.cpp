#include "pch.h"
#include "CEventManager.h"
#include "CPlayerController.h"
#include "COpponentController.h"
#include "SceneManager.h"
#include "CCamera.h"
#include "CDotween.h"
#include "SongManager.h"
#include "CCameraMove.h"

const float zoomDuration = 0.035f;
const float backDuration = 0.1f;

const float HUDZoomRatio = 1.065f;
const float objZoomRatio = 1.34f;

CEventManager::CEventManager()
	: CScript(SCRIPT_TYPE::EVENTMANAGER),
	m_isStarted(false), m_elapsed(0.f), m_camEventIdx(0), m_animEventIdx(0), m_isZoomStarted(false)
{
}

/*CEventManager::CEventManager(const CEventManager& _Origin)
	: CScript(_Origin),
	m_isStarted(false), m_elapsed(0.f), m_camEventIdx(0), m_animEventIdx(0)
{

}*/

CEventManager::~CEventManager()
{
}

void CEventManager::Begin()
{
	m_player = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"Player")->GetScript<CPlayerController>().Get();
	m_opponent = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"Opponent")->GetScript<COpponentController>().Get();

	m_cam = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"MainCamera")->Camera();

	m_camDotween = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"MainCamera")->GetScript<CDotween>().Get();

	m_spotLight[0] = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"PlayerSpotLight")->Light2D();
	m_spotLight[1] = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"OpponentSpotLight")->Light2D();

	m_spotLight[0]->SetLightColor(Vec3(0.f, 0.f, 0.f));
	m_spotLight[1]->SetLightColor(Vec3(0.f, 0.f, 0.f));

	m_directionalLight = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"DirectionalLight")->Light2D();

	// Parser 통해 정보 가져오기
	Parser::GetInstance()->LoadEventData(SONGNAME, this);

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

	std::sort(m_camEventVec.begin(), m_camEventVec.end(),
		[](const CameraEventInfo& a, const CameraEventInfo& b)
		{
			return a.timing < b.timing;
		});
}

void CEventManager::Tick()
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
			m_camDotween->DOMove(pos, 2.f)->SetEase(EaseOutExpo);
			++m_camEventIdx;
		}


		if (m_animEventIdx < m_animEventVec.size() && m_elapsed >= m_animEventVec[m_animEventIdx] * 0.001f)
		{
			// 애니메이션 Play
			m_player->PlayHEY();
			++m_animEventIdx;
		}

		// SpotLight 이벤트
		if (m_lightEventIdx < m_lightEventVec.size() && m_elapsed >= m_lightEventVec[m_lightEventIdx].start)
		{
			LightEventInfo currentLightInfo = m_lightEventVec[m_lightEventIdx];
			int idx = currentLightInfo.value;
			float lightTime = m_lightEventVec[m_lightEventIdx].end - m_lightEventVec[m_lightEventIdx].start;

			m_lightFlag |= (1 << idx);

			m_directionalLight->SetLightColor(Vec3(0.2f, 0.2f, 0.2f));
			m_spotLight[idx]->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
			m_spotLight[idx]->GetOwner()->GetScript<CDotween>()->DONothing(lightTime)->OnComplete([this, idx]()
				{
					m_spotLight[idx]->SetLightColor(Vec3(0.f, 0.f, 0.f));
					m_lightFlag &= ~(1 << idx);
					if (m_lightFlag == 0)
						m_directionalLight->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
				});

			m_lightEventIdx++;
		}

		// cameraZoom 이벤트
		m_zoomElapsed += DT;
		float oneBar = 60.f / BPM * 4;
		//float oneBar = 60.f / BPM * 7.f;		// 7 / 4

		if (!m_isZoomStarted)
		{
			m_isZoomStarted = true;
			// 여기서 maincam과 HUDcam의 beatHeat를 다른 파라미터로 각자 호출
			m_cam->GetOwner()->GetScript<CCameraMove>()->BeatHeat(zoomDuration, backDuration, objZoomRatio);
			SceneManager::GetInstance()->FindObjectByName(L"HUDCamera")
				->GetScript<CCameraMove>()->BeatHeat(zoomDuration, backDuration, HUDZoomRatio);
		}
		else if (m_zoomElapsed >= oneBar)
		{
			m_zoomElapsed -= oneBar;
			m_cam->GetOwner()->GetScript<CCameraMove>()->BeatHeat(zoomDuration, backDuration, objZoomRatio);
			SceneManager::GetInstance()->FindObjectByName(L"HUDCamera")
				->GetScript<CCameraMove>()->BeatHeat(zoomDuration, backDuration, HUDZoomRatio);
		}
	}		
}

void CEventManager::SaveToSceneFile(FILE* _File)
{
}

void CEventManager::LoadFromSceneFile(FILE* _File)
{
}
