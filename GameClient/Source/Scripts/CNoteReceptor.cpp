#include "pch.h"
#include "CNoteReceptor.h"
#include "CNote.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "AssetKeys.h"
#include "CAtlasAnimator.h"
#include "CPlayerController.h"

CNoteReceptor::CNoteReceptor(SCRIPT_TYPE _type)
	: CScript(_type),
	m_elapsed(0.f), m_appearElapsed(0.f), m_tapTimingIdx(0), m_pressedTimingIdx(0),
	m_isTapped(false), m_isMusicStart(false),
	m_noteSpeed(500.f), m_nrNum(-1), m_tapNoteIdx(0), m_pressNoteIdx(0)
{
}

CNoteReceptor::CNoteReceptor(const CNoteReceptor& _Origin)
	: CScript(_Origin),
	m_elapsed(0.f), m_appearElapsed(0.f), m_tapTimingIdx(0), m_pressedTimingIdx(0),
	m_isTapped(false), m_isMusicStart(false),
	m_noteSpeed(_Origin.m_noteSpeed), m_characterController(nullptr),
	m_dir(_Origin.m_dir), m_nrNum(_Origin.m_nrNum), m_tapNoteIdx(0), m_pressNoteIdx(0)
{
	tapTimings.resize(_Origin.tapTimings.size());
	pressTimings.resize(_Origin.pressTimings.size());
	m_tapNoteVec.resize(_Origin.m_tapNoteVec.size());
	m_pressNoteVec.resize(_Origin.m_pressNoteVec.size());

	for (size_t i = 0; i < _Origin.tapTimings.size(); ++i)
	{
		tapTimings[i] = _Origin.tapTimings[i];
	}

	for (size_t i = 0; i < _Origin.pressTimings.size(); ++i)
	{
		pressTimings[i] = _Origin.pressTimings[i];
	}
}

CNoteReceptor::~CNoteReceptor()
{

}

void CNoteReceptor::Begin()
{
	m_anim = AtlasAnimator();

	for (int i = 0; i < m_tapNoteVec.size(); ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Note_%d%d", m_nrNum, i);

		auto childVec = GetOwner()->GetChild();
		GameObject* obj = nullptr;

		for (auto& each : childVec)
		{
			if (each->GetName() == Buff)
			{
				obj = each.Get();
				break;
			}
		}
		obj->SetActive(false);

		m_tapNoteVec[i] = obj->GetScript<CNote>().Get();
	}

	for (int i = 0; i < m_pressNoteVec.size(); ++i)
	{
		{
			wchar_t Buff[50] = {};
			swprintf_s(Buff, L"Body_%d%d", m_nrNum, i);

			auto childVec = GetOwner()->GetChild();
			Ptr<GameObject> obj = nullptr;

			for (auto& each : childVec)
			{
				if (each->GetName() == Buff)
				{
					obj = each;
					break;
				}
			}

			obj->SetActive(false);

			m_pressNoteVec[i].body = obj->GetScript<CNote>().Get();
		}

		{
			wchar_t Buff[50] = {};
			swprintf_s(Buff, L"Tail_%d%d", m_nrNum, i);

			auto childVec = GetOwner()->GetChild();
			Ptr<GameObject> obj = nullptr;

			for (auto& each : childVec)
			{
				if (each->GetName() == Buff)
				{
					obj = each;
					break;
				}
			}

			obj->SetActive(false);

			m_pressNoteVec[i].tail = obj->GetScript<CNote>().Get();
		}
	}
}

void CNoteReceptor::Tick()
{

	// 노트를 appearTime 전에 setactive(true) 해주는 로직.
	// taptiming과 presstiming의 인덱스를 참조하면서 시간을 잰다.
	// 주의할 점 : 꼭 timingIdx 가 아닌 noteIdx로 참조할 것!!!
	m_appearElapsed += DT;
	float oneBar = 60.f / BPM * 5;

	float x = 2.f;		// 도달 x초 전에 노트 생성
	
	float appearTime = (m_appearElapsed + x - oneBar) * 1000.f;

	if (m_tapNoteIdx < tapTimings.size() && appearTime >= tapTimings[m_tapNoteIdx])
	{
		CNote* currentNote = m_tapNoteVec[m_tapNoteIdx++];

		float yOffset = m_noteSpeed * x;

		// 위치는 x초 * 속력으로 설정
		currentNote->GetOwner()->SetActive(true);
		currentNote->Transform()->SetRelativePosition(Vec3(0.f, -yOffset, -5.f));

		m_tapNoteQueue.push(currentNote);
	}

	if (m_pressNoteIdx < pressTimings.size() && appearTime >= pressTimings[m_pressNoteIdx].first)
	{
		CNote* bodyNote = m_pressNoteVec[m_pressNoteIdx].body;
		CNote* tailNote = m_pressNoteVec[m_pressNoteIdx].tail;

		float holdSec = pressTimings[m_pressNoteIdx].second * 0.001f;

		float holdPixelLength = m_noteSpeed * holdSec;

		float yOffset = m_noteSpeed * x;

		bodyNote->Transform()->SetRelativePosition(Vec3(0.f, -yOffset - holdPixelLength * 0.5f, -5.f));
		bodyNote->GetOwner()->SetActive(true);

		tailNote->Transform()->SetRelativePosition(Vec3(0.f, -yOffset - holdPixelLength, -5.f));
		tailNote->GetOwner()->SetActive(true);

		m_pressNoteQueue.push(m_pressNoteVec[m_pressNoteIdx]);

		++m_pressNoteIdx;
	}

}

void CNoteReceptor::MusicStart()
{
	m_isMusicStart = true;
}

void CNoteReceptor::DestroyTapNote()
{
	// 해당 노트 삭제
	if (!m_tapNoteQueue.empty())
	{
		CNote* pNote = m_tapNoteQueue.front();
		m_tapNoteQueue.pop();
		pNote->Destroy();
	}
}

void CNoteReceptor::DestroyPressNote()
{
	// 해당 Note 삭제
	if (!m_pressNoteQueue.empty())
	{
		LongNote pNote = m_pressNoteQueue.front();
		m_pressNoteQueue.pop();
		pNote.body->Destroy();
		pNote.tail->Destroy();
	}
}

