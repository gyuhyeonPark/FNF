#include "pch.h"
#include "PlayerNoteReceptor.h"
#include "CharacterController.h"
#include "CAtlasAnimator.h"
#include "AssetManager.h"
#include "CMissileScript.h"
#include "SceneManager.h"

PlayerNoteReceptor::PlayerNoteReceptor()
	: m_isPressed(false), m_orginScale{}
{
}

PlayerNoteReceptor::~PlayerNoteReceptor()
{
}

void PlayerNoteReceptor::Begin()
{
	NoteReceptor::Begin();

	string tag = "";

	// 이펙트 오브젝트를 생성한다.
	Ptr<ATexture> effectTexture = new ATexture;
	Ptr<AMaterial> effectMtrl = new AMaterial;
	Ptr<AMaterial> effectMtrlVolatile = new AMaterial;
	switch (m_dir)
	{
	case ARROW_DIR::LEFT:
		effectTexture = FIND(ATexture, L"effectpurplehold");
		effectMtrl = FIND(AMaterial, L"EffectPurpleHold");
		effectMtrlVolatile = FIND(AMaterial, L"EffectPurpleVolatile");
		tag = "Purple";
		break;
	case ARROW_DIR::DOWN:
		effectTexture = FIND(ATexture, L"effectbluehold");
		effectMtrl = FIND(AMaterial, L"EffectBlueHold");
		effectMtrlVolatile = FIND(AMaterial, L"EffectBlueVolatile");
		tag = "Blue";
		break;
	case ARROW_DIR::UP:
		effectTexture = FIND(ATexture, L"effectgreenhold");
		effectMtrl = FIND(AMaterial, L"EffectGreenHold");
		effectMtrlVolatile = FIND(AMaterial, L"EffectGreenVolatile");
		tag = "Green";
		break;
	case ARROW_DIR::RIGHT:
		effectTexture = FIND(ATexture, L"effectredhold");
		effectMtrl = FIND(AMaterial, L"EffectRedHold");
		effectMtrlVolatile = FIND(AMaterial, L"EffectRedVolatile");
		tag = "Red";
		break;
	default:
		break;
	}

	for (int i = 0; i < (UINT)PKeyState::END; ++i)
	{
		m_effectAnim[i] = new CAtlasAnimator;
		m_effectAnim[i]->LoadMapInfo(effectTexture);

		Ptr<GameObject> effectObj = new GameObject();
		effectObj->SetName(L"effect");
		effectObj->AddComponent(new CTransform);
		effectObj->AddComponent(new CMeshRenderer);
		effectObj->AddComponent(m_effectAnim[i].Get());
		effectObj->GetMeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
		
		if (i == (UINT)PKeyState::PRESSED)
			effectObj->GetMeshRenderer()->SetMtrl(effectMtrl);
		else
		{
			effectObj->GetMeshRenderer()->SetMtrl(effectMtrlVolatile);
			m_effectAnim[i]->SetAsVolatile();
		}
			
		GetOwner()->AddChild(effectObj);

		//effectObj->Transform()->SetRelativePosition(Transform()->GetRelativePosition() - Vec3(0.f, 0.f , 1.f));
		effectObj->Transform()->SetIndependentScale(true);
		effectObj->Transform()->SetRelativePosition(Vec3(0.f, 0.f, -1.f));
		effectObj->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 1.f));
		//CreateObject(effectObj, 1);
		//SceneManager::GetInstance()->GetCurrentScene()->AddObject(1, effectObj);	
	}

	for (int i = 0; i < 3; i++)
	{
		m_effectNames[i] = sharedEffectName[i] + tag;
	}

	m_orginScale = Transform()->GetRelativeScale();
}

void PlayerNoteReceptor::Tick()
{
	if (m_isMusicStart)
	{
		m_elapsed += DT;

		// 안전 검사: tapTimings[UINT(m_difficulty)]가 비었거나 인덱스가 범위를 벗어나면 처리할 것이 없음
		if (tapTimings[UINT(m_difficulty)].empty() || m_tapTimingIdx < 0 || m_tapTimingIdx >= static_cast<int>(tapTimings[UINT(m_difficulty)].size()))
			return;

		float currentNoteTiming = tapTimings[UINT(m_difficulty)][m_tapTimingIdx];

		// 노트가 지나가 버렸을 때
		if (m_elapsed * 1000.f - currentNoteTiming > BAD)
		{
			++m_tapTimingIdx;

			m_characterController->PlayAnimation
			(m_dir, true);
		
			if (pressTimings[UINT(m_difficulty)].size() > m_pressedTimingIdx 
				&& currentNoteTiming == pressTimings[UINT(m_difficulty)][m_pressedTimingIdx].first)
				++m_pressedTimingIdx;
		}
	}
}

void PlayerNoteReceptor::ActivateTapEvent()
{
	// 안전 검사: tapTimings[UINT(m_difficulty)]가 비었거나 인덱스가 범위를 벗어나면 처리할 것이 없음
	if (tapTimings[UINT(m_difficulty)].empty() || m_tapTimingIdx < 0 || m_tapTimingIdx >= static_cast<int>(tapTimings[UINT(m_difficulty)].size()))
		return;

	float currentTiming = tapTimings[UINT(m_difficulty)][m_tapTimingIdx];
	
	// ms 기준
	float diff = std::abs(currentTiming - m_elapsed * 1000.f);

	if (diff > BAD)
	{
		// note 입력 판정 x. 그대로 진행
		m_characterController->PlayAnimation(m_dir, true);
		m_anim->Play(tapArrowName[UINT(m_dir)], false);
	}
	else
	{
		// 노트 입력됨. 타이밍에 따라 점수 상이하게 부여
		m_characterController->PlayAnimation(m_dir);
		++m_tapTimingIdx;
		SOUNDMANAGER->PlayCorrect();

		m_anim->Play(confirmTapName[UINT(m_dir)], false);
		
		m_effectAnim[(UINT)PKeyState::TAP]->Play(m_effectNames[(UINT)EFFECT_ANIMKEY::START], false);
		m_effectAnim[(UINT)PKeyState::PRESSED]->Play(m_effectNames[(UINT)EFFECT_ANIMKEY::PRESSED], true);
		//m_effectAnim[(UINT)PKeyState::RELEASED]->Play(m_effectNames[(UINT)EFFECT_ANIMKEY::RELEASED], false);

		if (diff <= SICK)
		{
			GetOwner()->m_dotween->DOShakePosition(0.1f, 7.f, 3.f);
			GetOwner()->m_dotween->DOScale(m_orginScale * 1.2f, 0.1f)->SetEase(EaseOutExpo)->OnComplete([=]()
				{
					GetOwner()->m_dotween->DOScale(m_orginScale, 0.1f);
				});
		}
		else if (diff > SICK && diff <= GOOD)
		{

		}
		else if (diff > GOOD && diff <= BAD)
		{

		}

		if (m_pressedTimingIdx < pressTimings[UINT(m_difficulty)].size()
			&& currentTiming == pressTimings[UINT(m_difficulty)][m_pressedTimingIdx].first)
		{
			m_currentPressStartTiming = pressTimings[UINT(m_difficulty)][m_pressedTimingIdx].first;
			m_currentPressEdTiming = pressTimings[UINT(m_difficulty)][m_pressedTimingIdx].first + pressTimings[UINT(m_difficulty)][m_pressedTimingIdx].second;
			++m_pressedTimingIdx;
			m_isPressed = true;
		}
	}
}

void PlayerNoteReceptor::ActivatePressedEvent()
{
/*	if (m_elapsed * 1000.f >= m_currentPressEdTiming)
	{
		// press 성공
	}*/
}

void PlayerNoteReceptor::ActivateReleasedEvent()
{
	m_anim->Play(defaultArrowName[UINT(m_dir)], false);
	
	if (m_isPressed && m_currentPressStartTiming < m_elapsed * 1000.f && m_elapsed * 1000.f < m_currentPressEdTiming)
	{
		// press 실패
		m_characterController->PlayAnimation(m_dir, true);
		m_isPressed = false;
	}
}

void PlayerNoteReceptor::Shoot()
{
	// 미사일 역할의 오브젝트 생성
	GameObject* pObject = new GameObject;
	pObject->SetName(L"Missile");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	pObject->AddComponent(new CMissileScript(m_opponentObj));
	pObject->AddComponent(new CCollider2D);

	Vec3 vMyPos = Transform()->GetRelativePosition();
	//Vec3 vMyScale = Transform()->GetRelativeScale();
	//Vec3 vRotation = Transform()->GetRelativeRotation();
	//Vec3 vUp = Transform()->GetDir(DIR::UP);

	pObject->Transform()->SetRelativePosition(vMyPos);
	pObject->Transform()->SetRelativeScale(Vec3(60.f, 100.f, 1.f));

	pObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	pObject->MeshRenderer()->SetMtrl(AssetManager::GetInstance()->Find<AMaterial>(L"Std2DMtrl"));

	// 런타임 도중 GameObject를 생성할 땐 해당 함수를 호출한다. -> 다음 프레임에 적용 보장
	CreateObject(pObject, 1);
}
