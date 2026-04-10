#include "pch.h"
#include "COpponentNoteReceptor.h"
#include "CCharacterController.h"
#include "COpponentController.h"
#include "CAtlasAnimator.h"
#include "SceneManager.h"
#include "AssetManager.h"
#include "CNote.h"

COpponentNoteReceptor::COpponentNoteReceptor()
	: CNoteReceptor(SCRIPT_TYPE::OPPONENTNOTERECEPTOR), m_idleAnimDuration(0.f),
	m_idleAnimElapsed(0.f), m_isIdleTimerOn(false), m_effectName("")
{
}

COpponentNoteReceptor::~COpponentNoteReceptor()
{
}

void COpponentNoteReceptor::Begin()
{
	CNoteReceptor::Begin();

	m_characterController = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"Opponent")->GetScript<COpponentController>().Get();

	string tag = "";
	string tapTag = "";

	// Pressed 이펙트를 생성한다.
	Ptr<ATexture> effectTexture = new ATexture;
	Ptr<AMaterial> effectMtrl = new AMaterial;

	switch (m_dir)
	{
	case ARROW_DIR::LEFT:
		effectTexture = FIND(ATexture, L"effectpurplehold");
		effectMtrl = FIND(AMaterial, L"EffectPurpleHoldPressed")->Clone();
		tag = "Purple";
		tapTag = "purple";
		break;
	case ARROW_DIR::DOWN:
		effectTexture = FIND(ATexture, L"effectbluehold");
		effectMtrl = FIND(AMaterial, L"EffectBlueHoldPressed")->Clone();
		tag = "Blue";
		tapTag = "blue";
		break;
	case ARROW_DIR::UP:
		effectTexture = FIND(ATexture, L"effectgreenhold");
		effectMtrl = FIND(AMaterial, L"EffectGreenHoldPressed")->Clone();
		tag = "Green";
		tapTag = "green";
		break;
	case ARROW_DIR::RIGHT:
		effectTexture = FIND(ATexture, L"effectredhold");
		effectMtrl = FIND(AMaterial, L"EffectRedHoldPressed")->Clone();
		tag = "Red";
		tapTag = "red";
		break;
	default:
		break;
	}

	m_effectAnim = new CAtlasAnimator;
	m_effectAnim->LoadMapInfo(effectTexture);

	Ptr<GameObject> effectObj = new GameObject();
	effectObj->SetName(L"effect");
	effectObj->AddComponent(new CTransform);
	effectObj->AddComponent(new CMeshRenderer);
	effectObj->AddComponent(m_effectAnim.Get());
	effectObj->GetMeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	effectObj->GetMeshRenderer()->SetMaterial(effectMtrl);

	m_effectAnim->SetAsVolatile();
	GetOwner()->AddChild(effectObj);

	//effectObj->Transform()->SetRelativePosition(Transform()->GetRelativePosition() - Vec3(0.f, 0.f , 1.f));
	effectObj->Transform()->SetIndependentScale(true);
	effectObj->Transform()->SetRelativePosition(Vec3(0.f, 0.f, -6.f));
	effectObj->Transform()->SetRelativeScale(Vec3(80.f, 80.f, 1.f));

	m_effectName = "holdCover" + tag;
}

void COpponentNoteReceptor::Tick()
{
	CNoteReceptor::Tick();

	if (m_isMusicStart)
	{
		if (m_isIdleTimerOn)
		{
			m_idleAnimElapsed += DT;

			if (m_idleAnimDuration <= m_idleAnimElapsed * 1000.f)
			{
				m_characterController->IdleTimerOn();
				m_isIdleTimerOn = false;
				m_idleAnimElapsed = 0.f;

				// 여기서 다시 static애니메이션으로 돌아오도록 해주는 함수 호출
				AtlasAnimator()->Play(defaultArrowName[UINT(m_dir)], false);

				m_effectAnim->Discard();
			}
		}

		m_elapsed += DT;

		if (tapTimings.empty() || m_tapTimingIdx < 0 || m_tapTimingIdx >= static_cast<int>(tapTimings.size()))
			return;

		float currentNoteTiming = tapTimings[m_tapTimingIdx];

		// 노트가 지나가 버렸을 때
		if (m_elapsed * 1000.f > currentNoteTiming)
		{
			++m_tapTimingIdx;
			DestroyTapNote();

			if (m_pressedTimingIdx < pressTimings.size() &&
				currentNoteTiming == pressTimings[m_pressedTimingIdx].first)
			{
				m_characterController->PlayAnimation(m_dir, false, false);
				m_idleAnimDuration = pressTimings[m_pressedTimingIdx].second;
				m_isIdleTimerOn = true;
				m_pressedTimingIdx++;
				AtlasAnimator()->Play(tapArrowName[UINT(m_dir)], true);
				m_effectAnim->Play(m_effectName, true);

				// 해당 Note Vanish ON
				if (!m_pressNoteQueue.empty())
				{
					LongNote pNote = m_pressNoteQueue.front();
					pNote.body->ActivateVanish();
					pNote.tail->ActivateVanish();
					m_pressNoteQueue.pop();
				}
			}
			else
			{
				m_characterController->PlayAnimation(m_dir, false);
				AtlasAnimator()->Play(tapArrowName[UINT(m_dir)], false);
				AtlasAnimator()->SetAnimEndEvent([this]() { AtlasAnimator()->Play(defaultArrowName[UINT(m_dir)], false); });
				// 여기서 다시 static애니메이션으로 돌아오도록 해주는 함수 호출
			}
		}
	}
}

void COpponentNoteReceptor::SaveToSceneFile(FILE* _File)
{
}

void COpponentNoteReceptor::LoadFromSceneFile(FILE* _File)
{
}

void COpponentNoteReceptor::ActivateTapEvent()
{
}

void COpponentNoteReceptor::ActivatePressedEvent()
{
}

void COpponentNoteReceptor::ActivateReleasedEvent()
{
}
