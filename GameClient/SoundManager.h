#pragma once
#include "IResourceManager.h"

enum class SoundType
{
	BGM,
	PLAYER,
	OPPONENT,
	MISS1,
	MISS2,
	MISS3,
	THREE,
	TWO,
	ONE,
	GO,

	END,
};

enum class ChannelType
{
	BGM,
	PLAYER,
	OPPONENT,
	MISS,
	RSG,
	END,
};

/// <summary>
/// FMOD 라이브러리를 통한 사운드 입출력을 담당하는 클래스.
/// Sound 객체를 생성하고, 실행해주는 기능.
/// </summary>
class SoundManager : public IResourceManager, public SingletonClass<SoundManager>
{
	SINGLE(SoundManager);

public:
	virtual void Init() override;
	virtual void CreateResources(STAGENUM stageNumber) override;
	void Update();

public:
	void PlayStage(STAGENUM stageNumber);
	void PlayMiss();
	void PlayCorrect();

	void PlayRSG(SoundType type);

private:	
	FMOD::System* m_system;
	FMOD::DSP* m_pitchDSP;

	// sounds[stageNum][soundlist] -> sound 방식 접근
	std::vector<map<SoundType, FMOD::Sound*>> sounds;
	std::vector<FMOD::ChannelGroup*> m_channelGroups;
	uint8 m_version;

	string m_soundPath;

	bool missStart = false;
	float missTimer;
	float elapsed = 0.f;
};

