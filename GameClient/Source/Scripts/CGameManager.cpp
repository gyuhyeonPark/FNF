#include "pch.h"
#include "CGameManager.h"
#include "AssetManager.h"
#include "CPlayerController.h"
#include "COpponentController.h"
#include "CPlayerNoteReceptor.h"
#include "COpponentNoteReceptor.h"
#include "CAtlasAnimator.h"
#include "GameObject.h"
#include "SceneManager.h"

CGameManager::CGameManager()
	: CScript(SCRIPT_TYPE::GAMEMANAGER),
	m_diff(DIFFICULTIES::EASY), m_cam(nullptr), m_playerController(nullptr), m_opponentController(nullptr)
{
}

CGameManager::CGameManager(const CGameManager& _Origin)
	: CScript(SCRIPT_TYPE::GAMEMANAGER)
	, noteReceptors()
	, m_diff(_Origin.m_diff)
	, m_cam(_Origin.m_cam)
{
}

CGameManager::~CGameManager()
{
}

void CGameManager::Begin()
{
	m_playerController = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"Player")->GetScript<CPlayerController>().Get();
	m_opponentController = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"Opponent")->GetScript<COpponentController>().Get();
	
	m_gfAnim = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"GF")->AtlasAnimator();
	m_gfAnim->Play("GF Dancing Beat", true);

	m_cam = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"MainCamera")->Camera().Get();

	for (UINT i = 0; i < NOTE_SIZE; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"NoteReceptor_%d", i);
		noteReceptors[i] = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(Buff)->GetScript<CNoteReceptor>().Get();
	}
}

void CGameManager::Tick()
{
	KeyTapUpdate();
	KeyPressedUpdate();
	KeyReleasedUpdate();
}

void CGameManager::SetPlayerController(Ptr<CCharacterController> controller)
{
	for (UINT i = 0; i < (UINT)ARROW_DIR::END; i++)
	{
		noteReceptors[i]->SetCharacterController(controller);
	}
}

void CGameManager::SetOpponentController(Ptr<CCharacterController> controller)
{
	for (UINT i = (UINT)ARROW_DIR::END; i < NOTE_SIZE; i++)
	{
		noteReceptors[i]->SetCharacterController(controller);
	}
}

void CGameManager::MusicStart()
{
	for (UINT i = 0; i < NOTE_SIZE; i++)
	{
		noteReceptors[i]->MusicStart();
	}
}

void CGameManager::SetOpponent(Ptr<GameObject> obj)
{
	for (int i = 0; i < (UINT)ARROW_DIR::END; ++i)
	{
		noteReceptors[i]->SetOpponentObj(obj);
	}
}


void CGameManager::SaveToSceneFile(FILE* _File)
{
}

void CGameManager::LoadFromSceneFile(FILE* _File)
{
}

void CGameManager::KeyTapUpdate()
{
	// TAP Event
	if (KEY_TAP(KEY::LEFT))
	{	
		// 플레이어는 miss인지 정상 출력인지 판별하는 함수를 호출해준다.
		// 데이터는 noteReceptor[0 ~ 3] 가 들고 있기 때문에,
		// noteReceptor에서 판별 -> player에게 성공 / 실패 여부를 전달한다.
		//m_characterController->Play("BF NOTE LEFT");
		//noteReceptors[0]->m_anim->Play(tapArrowName[0], false);
		noteReceptors[(UINT)ARROW_DIR::LEFT]->ActivateTapEvent();
	}
	if (KEY_TAP(KEY::DOWN))
	{
		noteReceptors[(UINT)ARROW_DIR::DOWN]->ActivateTapEvent();
		//noteReceptors[(UINT)ARROW_DIR::DOWN]->m_anim->Play(tapArrowName[1], false);
		//m_characterController->Play("BF NOTE DOWN");
	}
	if (KEY_TAP(KEY::UP))
	{
		noteReceptors[(UINT)ARROW_DIR::UP]->ActivateTapEvent();
		//noteReceptors[(UINT)ARROW_DIR::UP]->m_anim->Play(tapArrowName[2], false);
		//m_characterController->Play("BF NOTE UP");
	}
	if (KEY_TAP(KEY::RIGHT))
	{
		noteReceptors[(UINT)ARROW_DIR::RIGHT]->ActivateTapEvent();
		//noteReceptors[(UINT)ARROW_DIR::RIGHT]->m_anim->Play(tapArrowName[3], false);
		//m_characterController->Play("BF NOTE RIGHT");
	}
}

void CGameManager::KeyPressedUpdate()
{
	// PRESSED Event
	if (KEY_PRESSED(KEY::LEFT))
	{
		noteReceptors[(UINT)ARROW_DIR::LEFT]->ActivatePressedEvent();
	}
	if (KEY_PRESSED(KEY::DOWN))
	{
		noteReceptors[(UINT)ARROW_DIR::DOWN]->ActivatePressedEvent();
	}
	if (KEY_PRESSED(KEY::UP))
	{
		noteReceptors[(UINT)ARROW_DIR::UP]->ActivatePressedEvent();
	}
	if (KEY_PRESSED(KEY::RIGHT))
	{
		noteReceptors[(UINT)ARROW_DIR::RIGHT]->ActivatePressedEvent();
	}
}

void CGameManager::KeyReleasedUpdate()
{
	// RELEASED Event
	if (KEY_RELEASED(KEY::LEFT))
	{
		noteReceptors[(UINT)ARROW_DIR::LEFT]->ActivateReleasedEvent();
	}
	if (KEY_RELEASED(KEY::DOWN))
	{
		noteReceptors[(UINT)ARROW_DIR::DOWN]->ActivateReleasedEvent();
	}
	if (KEY_RELEASED(KEY::UP))
	{
		noteReceptors[(UINT)ARROW_DIR::UP]->ActivateReleasedEvent();
	}
	if (KEY_RELEASED(KEY::RIGHT))
	{
		noteReceptors[(UINT)ARROW_DIR::RIGHT]->ActivateReleasedEvent();
	}
}
