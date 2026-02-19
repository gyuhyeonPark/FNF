#include "pch.h"
#include "GameManager.h"
#include "AssetManager.h"
#include "CPlayerController.h"
#include "PlayerNoteReceptor.h"
#include "OpponentNoteReceptor.h"
#include "CAtlasAnimator.h"
#include "GameObject.h"
#include "SceneManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::Init(DIFFICULTIES diff, Ptr<CCamera> cam)
{
	// 초기화 업무

	// 1. Note Receptor 오브젝트 생성
	// 스프라이트 출력, 컴포넌트 세팅 등...

	// 각 NR들은 하나의 이미지를 공유한다.
	Ptr<ATexture> nrTexture = new ATexture();
	nrTexture = AssetManager::GetInstance()->Find<ATexture>(L"notereceptor");

	for (UINT i = 0; i < NOTE_SIZE; i++)
	{
		Ptr<CAtlasAnimator> animator = new CAtlasAnimator;
		animator->LoadMapInfo(nrTexture);
		Ptr<NoteReceptor> nrCom;
		if (i < (UINT)ARROW_DIR::END)
			nrCom = new PlayerNoteReceptor();
		else
			nrCom = new OpponentNoteReceptor();

		nrCom->SetAnimator(animator);

		noteReceptors[i] = nrCom;

		Ptr<GameObject> nrObj = new GameObject();
		nrObj->SetName(L"NoteReceptor");
		nrObj->AddComponent(new CTransform);
		nrObj->AddComponent(new CMeshRenderer);
		nrObj->AddComponent(nrCom.Get());
		nrObj->AddComponent(animator.Get());
		
		Ptr<Dotween> dotween = new Dotween;
		nrObj->AddComponent(dotween.Get());
		nrObj->m_dotween = dotween.Get();
	
		//nrObj->AddAnimator(animator.Get());

		Ptr<AMesh> nrMesh = (AMesh*)AssetManager::GetInstance()->FindAsset(ASSET_TYPE::MESH, L"RectMesh").Get();
		Ptr<AMaterial> nrMaterial = AssetManager::GetInstance()->Find<AMaterial>(L"NoteReceptor");
		
		nrObj->GetMeshRenderer()->SetMesh(nrMesh);
		nrObj->GetMeshRenderer()->SetMtrl(nrMaterial);
		nrObj->GetTransform()->SetIndependentScale(true);
		nrObj->GetTransform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
		cam->GetOwner()->AddChild(nrObj);
		
		if (i < (UINT)ARROW_DIR::END)
		{
			nrObj->GetTransform()->SetRelativePosition(m_playerNRPos);
			m_playerNRPos.x += 100.f;
		}
		else
		{
			nrObj->GetTransform()->SetRelativePosition(m_opponentNRPos);
			m_opponentNRPos.x += 100.f;
		}

		//CreateObject(nrObj, 1);
		//SceneManager::GetInstance()->GetCurrentScene()->AddObject(1, nrObj);
	}

	// 곡에 대한 노트 정보를 각각의 Receptor에게 전달.
	PARSER->LoadNoteData(L"bopeebo", noteReceptors);

	// 난이도 설정 및 기본 Receptor Animation Setting
	for (UINT i = 0; i < NOTE_SIZE; i++)
	{
		noteReceptors[i]->SetDifficulties(diff);
		noteReceptors[i]->SetDir(ARROW_DIR(i % 4));
		noteReceptors[i]->m_anim->Play(defaultArrowName[i % 4], true);
		//noteReceptors[i]->Init();
	}
}

void GameManager::Tick()
{
	KeyTapUpdate();
	KeyPressedUpdate();
	KeyReleasedUpdate();
}

void GameManager::SetPlayerController(Ptr<CharacterController> controller)
{
	for (UINT i = 0; i < (UINT)ARROW_DIR::END; i++)
	{
		noteReceptors[i]->SetCharacterController(controller);
	}
}

void GameManager::SetOpponentController(Ptr<CharacterController> controller)
{
	for (UINT i = (UINT)ARROW_DIR::END; i < NOTE_SIZE; i++)
	{
		noteReceptors[i]->SetCharacterController(controller);
	}
}

void GameManager::MusicStart()
{
	for (UINT i = 0; i < NOTE_SIZE; i++)
	{
		noteReceptors[i]->MusicStart();
	}
}

void GameManager::SetOpponent(Ptr<GameObject> obj)
{
	for (int i = 0; i < (UINT)ARROW_DIR::END; ++i)
	{
		noteReceptors[i]->SetOpponentObj(obj);
	}
}


void GameManager::KeyTapUpdate()
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

void GameManager::KeyPressedUpdate()
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

void GameManager::KeyReleasedUpdate()
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
