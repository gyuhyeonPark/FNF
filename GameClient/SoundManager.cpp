#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager() 
	: missTimer(1.5f)
{

}

void SoundManager::Init()
{	 
	FMOD_RESULT result;

	sounds.resize(STAGENUM::STAGEEND);
	m_channelGroups.resize(int(ChannelType::END)); 

	result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK, "FMOD 객체 초기화 실패!");

	result = m_system->getVersion((unsigned int*)&m_version);
	assert(result == FMOD_OK, "FMOD 객체 초기화 실패!");

	result = m_system->init(70, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK, "FMOD 객체 초기화 실패!");

	FMOD::ChannelGroup* master = nullptr;
	m_system->getMasterChannelGroup(&master);
	
	for (UINT i = 0; i < int(ChannelType::END); i++)
	{
		m_system->createChannelGroup(nullptr, &m_channelGroups[i]);
		master->addGroup(m_channelGroups[i]);
	}

	wstring p = PathManager::GetInstance()->GetContentsPath();
	p += L"Resources/Sounds/";
	m_soundPath = std::string().assign(p.begin(), p.end());

	// 기본 공유 사운드 생성
	CreateResources(STAGENUM::SHARED);
}

void SoundManager::CreateResources(STAGENUM stageNumber)
{
	FMOD_RESULT result;

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
	}
	else
	{
		// bgm
		string bgmPath = m_soundPath + "BGM//" + StageFolderPath[stageNumber] + "//Inst.mp3";
		result = m_system->createSound((bgmPath).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::BGM]);

		// opponent
		string opponentPath = m_soundPath + "BGM//" + StageFolderPath[stageNumber] + "//Voices-dad.mp3";
		result = m_system->createSound((opponentPath).c_str(), FMOD_DEFAULT, 0, &sounds[stageNumber][SoundType::OPPONENT]);

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
			m_channelGroups[int(ChannelType::PLAYER)]->setVolume(1.f);
		}
	}
}

void SoundManager::PlayStage(STAGENUM stageNumber)
{
	CHECK(m_system->playSound(sounds[stageNumber][SoundType::BGM], m_channelGroups[int(ChannelType::BGM)], false, nullptr));
	CHECK(m_system->playSound(sounds[stageNumber][SoundType::PLAYER], m_channelGroups[int(ChannelType::PLAYER)], false, nullptr));
	CHECK(m_system->playSound(sounds[stageNumber][SoundType::OPPONENT], m_channelGroups[int(ChannelType::OPPONENT)], false, nullptr));

	//m_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &m_pitchDSP);

	//// ★ 중요: bypass 해제
	//m_pitchDSP->setBypass(false);

	//// 파라미터 설정
	//m_pitchDSP->setParameterFloat(
	//	FMOD_DSP_PITCHSHIFT_PITCH,
	//	1.1f
	//);

	////m_channelGroups[int(ChannelType::BGM)]->addDSP(FMOD_DSP_PITCHSHIFT_PITCH, m_pitchDSP);
	//m_channelGroups[int(ChannelType::PLAYER)]->addDSP(FMOD_DSP_PITCHSHIFT_PITCH, m_pitchDSP);
}

void SoundManager::PlayMiss()
{
	int rndIdx = rand() % 3 + 3;		// 3 ~ 5
	m_system->playSound(sounds[STAGENUM::SHARED][SoundType(rndIdx)], m_channelGroups[int(ChannelType::MISS)], false, nullptr);

	m_channelGroups[int(ChannelType::PLAYER)]->setMute(true);
	missStart = true;
	elapsed = 0.f;
}

void SoundManager::PlayCorrect()
{
	/// miss에서 줄어든 사운드를 다시 복구하기
	m_channelGroups[int(ChannelType::PLAYER)]->setMute(false);
	missStart = false;
	elapsed = 0.f;
}


void SoundManager::PlayRSG(SoundType type)
{
	m_system->playSound(sounds[STAGENUM::SHARED][type], m_channelGroups[int(ChannelType::RSG)], false, nullptr);
}
