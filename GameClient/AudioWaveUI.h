#pragma once
#include "EditorUI.h"
#include "../GameClient/Source/Scripts/CNoteReceptor.h"
#include "ISongDataUI.h"
#include "IResourceManager.h"

struct WaveSample
{
	int8_t min;
	int8_t max;
};

enum class EVENT_TYPE
{
	FOCUS_CAMERA,
	//PLAY_ANIMATION,
	SPOT_LIGHT,
	END,
};

class CNote;

class AudioWaveUI : public EditorUI
{
public:
	AudioWaveUI();
	virtual ~AudioWaveUI();

public:
	virtual void Tick_UI() override;

	void SetCurrentPos(int _pos) { m_currentPos = _pos; }

private:
	bool LoadWaveform(const std::string& path, int _idx);
	void DrawWaveform(int _idx);

	void ChangeSong(DWORD_PTR _ListUI);

	void DetectPicking();
	void DetectEventPicking();

	void SaveMap();
	void LoadMap(const wstring& fileName);

	wstring OpenFileDialog();

private:
	std::vector<WaveSample> m_waveform[2];
	std::vector<pair<string, string>> AudioWaveNameVec;

	std::list<int> eventTimings[(int)EVENT_TYPE::END][2];

	STAGENUM m_currentSong;
	DIFFICULTIES m_currentDiff;

	int m_currentPos;
	int m_sliderPos;

	ImVec2 playerPos;
	ImVec2 opponentPos;

	bool m_eventMode;
	bool m_recordMode;

	//pair<TapIter, std::list<int>*> m_pickEventInfo;

	ISongDataUI* m_songDataUIs[(UINT)EDIT_TYPE::END];
	EDIT_TYPE m_currentType;
};

