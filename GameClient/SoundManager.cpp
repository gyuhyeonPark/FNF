#include "pch.h"
#include "SoundManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "CStageTransition.h"

SoundManager::SoundManager() 
	: missTimer(1.5f), missStart(false), m_musicStart(false), elapsed(0.f), m_currentStage(STAGENUM::STAGEEND)
{
}

void SoundManager::Init()
{	 
	FMOD_RESULT result;

	sounds.resize(STAGENUM::STAGEEND);
	m_channels.resize(int(ChannelType::END));

	for (UINT i = 0; i < int(ChannelType::END); i++)
	{
		m_channels[i] = nullptr;
	}

	result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK, "FMOD 객체 초기화 실패!");

	result = m_system->getVersion((unsigned int*)&m_version);
	assert(result == FMOD_OK, "FMOD 객체 초기화 실패!");

	result = m_system->init(70, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK, "FMOD 객체 초기화 실패!");

	wstring p = PathManager::GetInstance()->GetContentsPath();
	p += L"Resources/Sounds/";
	m_soundPath = std::string().assign(p.begin(), p.end());

	// 기본 공유 사운드 생성
	CreateResources(STAGENUM::SHARED);
}

void SoundManager::CreateResources(STAGENUM stageNumber)
{
	FMOD_RESULT result;

	string opponentTag = "";

	if (!sounds[stageNumber].empty())
		return;

	if (stageNumber == STAGENUM::SHARED)
	{
		string path = m_soundPath + StageFolderPath[stageNumber] + "//miss" + "//missnote1.mp3";
		result = m_system->createSound((path).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::MISS1]);

		path = m_soundPath + StageFolderPath[stageNumber] + "//miss" + "//missnote2.mp3";
		result = m_system->createSound((path).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::MISS2]);

		path = m_soundPath + StageFolderPath[stageNumber] + "//miss" + "//missnote3.mp3";
		result = m_system->createSound((path).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::MISS3]);

		path = m_soundPath + StageFolderPath[stageNumber] + "//countdown" + "//introTHREE.mp3";
		result = m_system->createSound((path).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::THREE]);

		path = m_soundPath + StageFolderPath[stageNumber] + "//countdown" + "//introTWO.mp3";
		result = m_system->createSound((path).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::TWO]);

		path = m_soundPath + StageFolderPath[stageNumber] + "//countdown" + "//introONE.mp3";
		result = m_system->createSound((path).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::ONE]);

		path = m_soundPath + StageFolderPath[stageNumber] + "//countdown" + "//introGO.mp3";
		result = m_system->createSound((path).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::GO]);

		path = m_soundPath + StageFolderPath[stageNumber] + "//confirmMenu.mp3";
		result = m_system->createSound((path).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::MAINENTER]);
	}
	else if (stageNumber == STAGENUM::MAINMENU)
	{
		string path = m_soundPath + "BGM//" + StageFolderPath[stageNumber] + "//freakyMenu.mp3";
		result = m_system->createSound((path).c_str(), FMOD_LOOP_NORMAL, 0, &sounds[stageNumber][SoundType::BGM]);
	}
	else
	{
		// bgm
		string bgmPath = m_soundPath + "BGM//" + StageFolderPath[stageNumber] + "//Inst.mp3";
		result = m_system->createSound((bgmPath).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::BGM]);

		if (stageNumber == STAGENUM::SENPAI)
		{
			string opponentPath = m_soundPath + "BGM//" + StageFolderPath[stageNumber] + "//Voices-senpai.mp3";
			result = m_system->createSound((opponentPath).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::OPPONENT]);
		}
		else
		{
			// opponent
			string opponentPath = m_soundPath + "BGM//" + StageFolderPath[stageNumber] + "//Voices-dad.mp3";
			result = m_system->createSound((opponentPath).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::OPPONENT]);
		}

		// player
		string playerPath = m_soundPath + "BGM//" + StageFolderPath[stageNumber] + "//Voices-bf.mp3";
		result = m_system->createSound((playerPath).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::PLAYER]);
	}
}

void SoundManager::Update()
{
	m_system->update();

	if (missStart)
	{
		elapsed += DT;		// BPM을 가미한 적절한 값을 timer 설정
		if (elapsed >= missTimer)
		{
			elapsed = 0.f;
			missStart = false;
			m_channels[int(ChannelType::PLAYER)]->setVolume(1.f);
		}
	}

	if (m_musicStart)
	{
		bool isPlaying = false;
		FMOD_RESULT result = m_channels[int(ChannelType::BGM)]->isPlaying(&isPlaying);

		if (!isPlaying)
		{
			m_musicStart = false;
			Ptr<GameObject> obj = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"StageChanger");
			if (obj != nullptr)
			{
				obj->GetScript<CStageTransition>()->TriggerOn();
			}
		}
	}
}

void SoundManager::ChangeStage(STAGENUM stageNumber, bool _isPlay)
{
	m_currentStage = stageNumber;
	m_musicStart = _isPlay;

	// 기존 채널 stop
	for (int i = 0; i < int(ChannelType::END); i++)
	{
		if (m_channels[i])
			m_channels[i]->stop();
	}

	// BGM
	m_system->playSound(
		sounds[stageNumber][SoundType::BGM],
		nullptr,
		false,
		&m_channels[int(ChannelType::BGM)]
	);

	if (stageNumber != STAGENUM::MAINMENU)
	{
		m_system->playSound(
			sounds[stageNumber][SoundType::PLAYER],
			nullptr,
			false,
			&m_channels[int(ChannelType::PLAYER)]
		);

		m_system->playSound(
			sounds[stageNumber][SoundType::OPPONENT],
			nullptr,
			false,
			&m_channels[int(ChannelType::OPPONENT)]
		);
	}

	if (!m_musicStart)
	{
		// 기존 채널 stop
		for (int i = 0; i < int(ChannelType::END); i++)
		{
			if (m_channels[i])
				m_channels[i]->setPaused(true);
		}
	}
}

void SoundManager::PlayMiss()
{
	int rndIdx = rand() % 3 + 3;		// 3 ~ 5
	m_system->playSound(sounds[STAGENUM::SHARED][SoundType(rndIdx)], nullptr, false, nullptr);

	m_channels[int(ChannelType::PLAYER)]->setMute(true);
	missStart = true;
	elapsed = 0.f;
}

void SoundManager::PlayCorrect()
{
	/// miss에서 줄어든 사운드를 다시 복구하기
	m_channels[int(ChannelType::PLAYER)]->setMute(false);
	missStart = false;
	elapsed = 0.f;
}


void SoundManager::PlaySFX(SoundType type)
{
	m_system->playSound(sounds[STAGENUM::SHARED][type], nullptr, false, nullptr);
}

void SoundManager::MuteAll(bool _boolen)
{
	FMOD::ChannelGroup* masterGroup = nullptr;
	m_system->getMasterChannelGroup(&masterGroup);
	masterGroup->setMute(_boolen);
}

void SoundManager::MuteSong(bool _boolen)
{
	m_channels[int(ChannelType::BGM)]->setMute(_boolen);
	m_channels[int(ChannelType::PLAYER)]->setMute(_boolen);
	m_channels[int(ChannelType::OPPONENT)]->setMute(_boolen);
}

void SoundManager::Pause()
{
	m_channels[int(ChannelType::BGM)]->setPaused(true);
	m_channels[int(ChannelType::PLAYER)]->setPaused(true);
	m_channels[int(ChannelType::OPPONENT)]->setPaused(true);

	m_musicStart = false;
}

void SoundManager::Resume()
{
	bool isPlaying = true;
	FMOD_RESULT result = m_channels[int(ChannelType::BGM)]->isPlaying(&isPlaying);

	if (!isPlaying)
		ChangeStage(m_currentStage, true);

	m_channels[int(ChannelType::BGM)]->setPaused(false);
	m_channels[int(ChannelType::PLAYER)]->setPaused(false);
	m_channels[int(ChannelType::OPPONENT)]->setPaused(false);

	m_musicStart = true;
}

int SoundManager::GetTotalLength(STAGENUM stageNum)
{
	unsigned int length = 0;
	sounds[stageNum][SoundType::BGM]->getLength(&length, FMOD_TIMEUNIT_MS);

	return length;
}

void SoundManager::SetPosition(int currentTime)
{
	m_channels[int(ChannelType::BGM)]->setPosition(currentTime, FMOD_TIMEUNIT_MS);
	m_channels[int(ChannelType::PLAYER)]->setPosition(currentTime, FMOD_TIMEUNIT_MS);
	m_channels[int(ChannelType::OPPONENT)]->setPosition(currentTime, FMOD_TIMEUNIT_MS);
}

int SoundManager::GetPosition()
{
	unsigned int pos = 0;
	m_channels[int(ChannelType::BGM)]->getPosition(&pos, FMOD_TIMEUNIT_MS);
	return pos;
}

