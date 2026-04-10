#pragma once
#include "SingletonClass.h"
class SongManager : public SingletonClass<SongManager>
{
	SINGLE(SongManager);
public:
	virtual ~SongManager();

public:
	void ChangeSong(STAGENUM _song) { m_currentStageNum = _song; }
	void SetDiff(DIFFICULTIES _diff) { m_currentDiff = _diff; }

	STAGENUM GetCurrentStageNum() { return m_currentStageNum; }
	float GetCurrentBPM() { return m_currentBPM[m_currentStageNum]; }
	wstring GetCurrentSongName() { return (*m_currentSongVec)[m_currentStageNum]; }

	DIFFICULTIES GetCurrentDiff() { return m_currentDiff; }

	const vector<wstring> GetSongVec() { return m_songVec; }

	void CustomON();

private:
	STAGENUM m_currentStageNum;
	vector<float> m_currentBPM;
	vector<wstring> m_songVec;
	vector<wstring> m_customSongVec;

	vector<wstring>* m_currentSongVec;

	DIFFICULTIES m_currentDiff;

	bool m_customMapMode = false;
};

#define BPM SongManager::GetInstance()->GetCurrentBPM()
#define DIFF SongManager::GetInstance()->GetCurrentDiff()
#define SONGNAME SongManager::GetInstance()->GetCurrentSongName()
#define SONGNUM SongManager::GetInstance()->GetCurrentStageNum()