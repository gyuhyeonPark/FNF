#include "pch.h"
#include "SongManager.h"
#include "ListUI.h"

SongManager::SongManager()
	: m_currentStageNum(STAGENUM::STAGEEND), m_currentDiff(DIFFICULTIES::END)
{
	m_currentBPM.resize(STAGENUM::STAGEEND);
	m_currentBPM =
	{
		0.f,
		MAINMENU_BPM,
		BOPEEBO_BPM,
		FRESH_BPM,
		DADBATTLE_BPM,
		SENPAI_BPM,
		SQUARE_BPM,
		WHIPLASH_BPM,
		LTR_BPM,
	};

	m_songVec.resize(STAGENUM::STAGEEND);
	m_songVec =
	{
		L"shared",
		L"freakyMenu",
		L"bopeebo",
		L"fresh",
		L"dadbattle",
		L"senpai",
		L"square",
		L"whiplash",
		L"Custom\\LTR",
	};

	m_customSongVec.resize(STAGENUM::STAGEEND);
	m_customSongVec =
	{
		L"Custom\\shared",
		L"Custom\\freakyMenu",
		L"Custom\\bopeebo",
		L"Custom\\fresh",
		L"Custom\\dadbattle",
		L"Custom\\senpai",
		L"Custom\\square",
		L"Custom\\whiplash",
		L"Custom\\LTR",
	};

	m_currentSongVec = &m_songVec;
}

SongManager::~SongManager()
{
}

void SongManager::CustomON()
{
	m_currentSongVec = &m_customSongVec;
}
