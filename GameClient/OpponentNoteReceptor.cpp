#include "pch.h"
#include "OpponentNoteReceptor.h"
#include "CharacterController.h"
#include "CAtlasAnimator.h"

OpponentNoteReceptor::OpponentNoteReceptor()
{
}

OpponentNoteReceptor::~OpponentNoteReceptor()
{
}

void OpponentNoteReceptor::Tick()
{
	if (m_isMusicStart)
	{
		m_elapsed += DT;

		// 안전 검사: tapTimings[UINT(m_difficulty)]가 비었거나 인덱스가 범위를 벗어나면 처리할 것이 없음
		if (tapTimings[UINT(m_difficulty)].empty() || m_tapTimingIdx < 0 || m_tapTimingIdx >= static_cast<int>(tapTimings[UINT(m_difficulty)].size()))
			return;

		float currentNoteTiming = tapTimings[UINT(m_difficulty)][m_tapTimingIdx];

		// 노트가 지나가 버렸을 때
		if (m_elapsed * 1000.f > currentNoteTiming)
		{
			++m_tapTimingIdx;

			m_characterController->PlayAnimation(m_dir, false);
			//m_anim->Play(confirmTapName[UINT(m_dir)], false);
		}
	}
}

void OpponentNoteReceptor::ActivateTapEvent()
{
}

void OpponentNoteReceptor::ActivatePressedEvent()
{
}

void OpponentNoteReceptor::ActivateReleasedEvent()
{
}
