#include "pch.h"
#include "CPlayerNoteReceptor.h"
#include "CCharacterController.h"
#include "CAtlasAnimator.h"
#include "AssetManager.h"
#include "CMissileScript.h"
#include "SceneManager.h"
#include "CPlayerController.h"
#include "CDotween.h"
#include "CComboManager.h"
#include "CNote.h"

CPlayerNoteReceptor::CPlayerNoteReceptor()
	: CNoteReceptor(SCRIPT_TYPE::PLAYERNOTERECEPTOR),
	m_isPressed(false), m_orginScale{}, m_isWellPressed(false)
{
}

CPlayerNoteReceptor::CPlayerNoteReceptor(const CPlayerNoteReceptor& _Orgin)
	: CNoteReceptor(_Orgin)
	, m_orginScale(_Orgin.m_orginScale), m_isPressed(false)
	, m_currentPressEdTiming(0.f)
	, m_currentPressStartTiming(0.f), m_isWellPressed(false)
{
/*	for (int i = 0; i < (UINT)PKeyState::END; ++i)
	{
		m_effectAnim[i] = _Orgin.m_effectAnim[i];
	}*/

	for (int i = 0; i < 3; ++i)
	{
		m_effectNames[i] = _Orgin.m_effectNames[i];
	}
}

CPlayerNoteReceptor::~CPlayerNoteReceptor()
{
}

void CPlayerNoteReceptor::Begin()
{
	CNoteReceptor::Begin();

	m_characterController = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"Player")->GetScript<CPlayerController>().Get();

	string tag = "";
	string tapTag = "";

	// 이펙트 오브젝트를 생성한다.
	Ptr<ATexture> effectTexture = new ATexture;
	Ptr<AMaterial> effectMtrl = new AMaterial;
	wstring tags[3] = { L"Tap", L"Pressed", L"Released" };

	// shader와 texture가 같은 Mtrl을 공유하면 sprite 중복이 발생함. 
	// shader를 같이 사용하는데, scalar를 1로 만들면 원치 않던 animator의 출력이 되는걸로 추정.
	for (int i = 0; i < (UINT)PKeyState::END; ++i)
	{
		switch (m_dir)
		{
		case ARROW_DIR::LEFT:
			effectTexture = FIND(ATexture, L"effectpurplehold");
			effectMtrl = FIND(AMaterial, L"EffectPurpleHold" + tags[i])->Clone();
			tag = "Purple";
			tapTag = "purple";
			break;
		case ARROW_DIR::DOWN:
			effectTexture = FIND(ATexture, L"effectbluehold");
			effectMtrl = FIND(AMaterial, L"EffectBlueHold" + tags[i])->Clone();
			tag = "Blue";
			tapTag = "blue";
			break;
		case ARROW_DIR::UP:
			effectTexture = FIND(ATexture, L"effectgreenhold");
			effectMtrl = FIND(AMaterial, L"EffectGreenHold" + tags[i])->Clone();
			tag = "Green";
			tapTag = "green";
			break;
		case ARROW_DIR::RIGHT:
			effectTexture = FIND(ATexture, L"effectredhold");
			effectMtrl = FIND(AMaterial, L"EffectRedHold" + tags[i])->Clone();
			tag = "Red";
			tapTag = "red";
			break;
		default:
			break;
		}

		m_effectAnim[i] = new CAtlasAnimator;

		if (i == (UINT)PKeyState::TAP)
			m_effectAnim[i]->LoadMapInfo(FIND(ATexture, L"tapEffects"));
		else
			m_effectAnim[i]->LoadMapInfo(effectTexture);

		Ptr<GameObject> effectObj = new GameObject();
		effectObj->SetName(L"effect");
		effectObj->AddComponent(new CTransform);
		effectObj->AddComponent(new CMeshRenderer);
		effectObj->AddComponent(m_effectAnim[i].Get());
		effectObj->GetMeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
		effectObj->GetMeshRenderer()->SetMaterial(effectMtrl);
		
		m_effectAnim[i]->SetAsVolatile();		
		GetOwner()->AddChild(effectObj);

		//effectObj->Transform()->SetRelativePosition(Transform()->GetRelativePosition() - Vec3(0.f, 0.f , 1.f));
		effectObj->Transform()->SetIndependentScale(true);
		effectObj->Transform()->SetRelativePosition(Vec3(0.f, 0.f, -6.f));

		if (i == (UINT)PKeyState::PRESSED)
			effectObj->Transform()->SetRelativeScale(Vec3(80.f, 80.f, 1.f));
		else
			effectObj->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 1.f));

		//CreateObject(effectObj, 1);
		//SceneManager::GetInstance()->GetCurrentScene()->AddObject(1, effectObj);	
	}

	for (int i = 0; i < 2; i++)
	{
		m_effectNames[i] = sharedEffectName[i] + tapTag;
	}

	for (int i = 2; i < 4; i++)
	{
		m_effectNames[i] = sharedEffectName[i] + tag;
	}

	m_orginScale = Transform()->GetRelativeScale();
}

void CPlayerNoteReceptor::Tick()
{
	CNoteReceptor::Tick();

	if (m_isMusicStart)
	{
		m_elapsed += DT;

		// 안전 검사: tapTimings가 비었거나 인덱스가 범위를 벗어나면 처리할 것이 없음
		if (tapTimings.empty() || m_tapTimingIdx < 0 || m_tapTimingIdx >= static_cast<int>(tapTimings.size()))
			return;

		float currentNoteTiming = tapTimings[m_tapTimingIdx];

		// 노트가 지나가 버렸을 때
		if (m_elapsed * 1000.f - currentNoteTiming > BAD)
		{
			++m_tapTimingIdx;
			if (!m_tapNoteQueue.empty())
				m_tapNoteQueue.pop();		// destroy는 Note에 맡기고 Pop만 해주기

			m_characterController->PlayAnimation(m_dir, true);
		
			CComboManager::GetInstance()->ResetCombo(JudgeMent::E_SHIT);

			if (pressTimings.size() > m_pressedTimingIdx
				&& currentNoteTiming == pressTimings[m_pressedTimingIdx].first)
			{
				++m_pressedTimingIdx;

				if (!m_pressNoteQueue.empty())
				{
					// pop 해주기 전엔 상수 버퍼 값 할당
					m_pressNoteQueue.pop();
				}
			}	
			return;
		}
	}
}

void CPlayerNoteReceptor::ActivateTapEvent()
{
	m_anim = AtlasAnimator();
	m_anim->Play(tapArrowName[UINT(m_dir)], false);

	// 안전 검사: tapTimings가 비었거나 인덱스가 범위를 벗어나면 처리할 것이 없음
	if (tapTimings.empty() || m_tapTimingIdx < 0 || m_tapTimingIdx >= static_cast<int>(tapTimings.size()))
		return;

	float currentTiming = tapTimings[m_tapTimingIdx];

	// ms 기준
	float diff = std::abs(currentTiming - m_elapsed * 1000.f);
	if (diff > BAD)
	{
		// note 입력 판정 x. 그대로 진행
		m_characterController->PlayAnimation(m_dir, true);
		CComboManager::GetInstance()->ResetCombo(JudgeMent::E_SHIT);
	}
	else
	{
		// 노트 입력됨. 타이밍에 따라 점수 상이하게 부여
		m_characterController->PlayAnimation(m_dir, false, false);
		++m_tapTimingIdx;
		SOUNDMANAGER->PlayCorrect();
		m_anim->Play(confirmTapName[UINT(m_dir)], false);
		m_isWellPressed = true;

		// 해당 노트 삭제
		DestroyTapNote();

		if (diff <= SICK)
		{
			int rndIdx = rand() % 2;		// 0 or 1
			//m_effectAnim[(UINT)PKeyState::TAP]->Play(m_effectNames[rndIdx], false);
			m_effectAnim[(UINT)PKeyState::RELEASED]->Play(m_effectNames[(UINT)EFFECT_ANIMKEY::RELEASED], false);
			GetOwner()->GetScript<CDotween>()->DOShakePosition(0.1f, 7.f, 3.f);
			GetOwner()->GetScript<CDotween>()->DOScale(m_orginScale * 1.2f, 0.1f)->SetEase(EaseOutExpo)->OnComplete([=]()
				{
					GetOwner()->GetScript<CDotween>()->DOScale(m_orginScale, 0.1f);
				});
			CComboManager::GetInstance()->AddCombo(JudgeMent::E_SICK);
		}
		else if (diff > SICK && diff <= GOOD)
		{
			CComboManager::GetInstance()->AddCombo(JudgeMent::E_GOOD);
		}
		else if (diff > GOOD && diff <= BAD)
		{
			CComboManager::GetInstance()->ResetCombo(JudgeMent::E_BAD);
		}

		if (m_pressedTimingIdx < pressTimings.size()
			&& abs(currentTiming - pressTimings[m_pressedTimingIdx].first) <= FLT_EPSILON)
		{
			m_currentPressStartTiming = pressTimings[m_pressedTimingIdx].first;
			m_currentPressEdTiming = pressTimings[m_pressedTimingIdx].first + pressTimings[m_pressedTimingIdx].second;
			++m_pressedTimingIdx;
			m_isPressed = true;
			m_effectAnim[(UINT)PKeyState::PRESSED]->Play(m_effectNames[(UINT)EFFECT_ANIMKEY::PRESSED], true);
			// 해당 Note Vanish ON
			if (!m_pressNoteQueue.empty())
			{
				LongNote pNote = m_pressNoteQueue.front();
				pNote.body->ActivateVanish();
				pNote.tail->ActivateVanish();
			}
		}
	}
}

void CPlayerNoteReceptor::ActivatePressedEvent()
{
	if (m_elapsed * 1000.f >= m_currentPressEdTiming - 50.f && m_isPressed)
	{
		// press 성공
		m_effectAnim[(UINT)PKeyState::PRESSED]->Discard();
		m_effectAnim[(UINT)PKeyState::RELEASED]->Play(m_effectNames[(UINT)EFFECT_ANIMKEY::RELEASED], false);
		m_isPressed = false;

		m_pressNoteQueue.pop();
	}

/*	// Safe-Fail
	if (m_isPressed && m_elapsed * 1000.f > m_currentPressEdTiming + 50.f)
	{
		m_effectAnim[PRESSED]->Discard();
		m_effectAnim[(UINT)PKeyState::RELEASED]->Play(m_effectNames[(UINT)EFFECT_ANIMKEY::RELEASED], false);
		m_isPressed = false;
	}*/
}

void CPlayerNoteReceptor::ActivateReleasedEvent()
{
	m_anim->Play(defaultArrowName[UINT(m_dir)], false);
	
	if (m_isPressed && m_currentPressStartTiming < m_elapsed * 1000.f && m_elapsed * 1000.f < m_currentPressEdTiming)
	{
		// press 실패
		m_characterController->PlayAnimation(m_dir, true);
		CComboManager::GetInstance()->ResetCombo(JudgeMent::E_SHIT);
		m_isPressed = false;
		m_effectAnim[(UINT)PKeyState::PRESSED]->Discard();

		DestroyPressNote();
	}
	else
	{
		m_characterController->IdleTimerOn();
	}
}

void CPlayerNoteReceptor::SaveToSceneFile(FILE* _File)
{
}

void CPlayerNoteReceptor::LoadFromSceneFile(FILE* _File)
{
}

void CPlayerNoteReceptor::Shoot()
{
	// 미사일 역할의 오브젝트 생성
	GameObject* pObject = new GameObject;
	pObject->SetName(L"Missile");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	pObject->AddComponent(new CMissileScript);
	pObject->AddComponent(new CCollider2D);

	Vec3 vMyPos = Transform()->GetRelativePosition();
	//Vec3 vMyScale = Transform()->GetRelativeScale();
	//Vec3 vRotation = Transform()->GetRelativeRotation();
	//Vec3 vUp = Transform()->GetDir(DIR::UP);

	pObject->Transform()->SetRelativePosition(vMyPos);
	pObject->Transform()->SetRelativeScale(Vec3(60.f, 100.f, 1.f));

	pObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(AssetManager::GetInstance()->Find<AMaterial>(L"Std2DMtrl"));

	// 런타임 도중 GameObject를 생성할 땐 해당 함수를 호출한다. -> 다음 프레임에 적용 보장
	CreateObject(pObject, 1);
}
