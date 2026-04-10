#pragma once
#include "ISongDataUI.h"
#include "../GameClient/Source/Scripts/CNoteReceptor.h"

struct RecordedTapNote
{
	Ptr<GameObject> obj;
	int time;          // 생성 시점 (pixel 기준)
	ARROW_DIR dir;
};

struct RecordedPressNote
{
	Ptr<GameObject> obj[3];		// Note - Body - Tail
	int time;				// 생성 시점 (pixel 기준)
	int hold;
	ARROW_DIR dir;
};

typedef std::list<int>::iterator TapIter;
typedef std::list<std::pair<int, int>>::iterator PressIter;

#pragma region Hashes
struct TapIterHash
{
	size_t operator()(const TapIter& iter) const noexcept
	{
		return std::hash<const void*>()(&(*iter));
	}
};

struct TapIterEqual
{
	bool operator()(const TapIter& a, const TapIter& b) const noexcept
	{
		return &(*a) == &(*b);
	}
};
struct PressIterHash
{
	size_t operator()(const PressIter& iter) const noexcept
	{
		return std::hash<const void*>()(&(*iter));
	}
};
struct PressIterEqual
{
	bool operator()(const PressIter& a, const PressIter& b) const noexcept
	{
		return &(*a) == &(*b);
	}
};
#pragma endregion

class NoteDataUI : public ISongDataUI
{
public:
	NoteDataUI();
	virtual ~NoteDataUI();

public:
	virtual void Tick_UI() {};

public:
	virtual void Update() override;
	virtual void DrawOnWaveForm(int _idx) override;
	virtual void Record() override;
	virtual void Undo() override;
	virtual void Delete() override;
	virtual void ClearList() override;

	virtual void Save(Document& doc) override;
	virtual void Load(Document& doc) override;
	virtual void DetectPicking() override;

private:
	void InsertTapNode(ARROW_DIR dir);
	void InsertPressNode(ARROW_DIR dir);

	Ptr<GameObject> CreateRecordTapNote(ARROW_DIR dir, int _pos, bool _charidx, bool isHead = true);
	Ptr<GameObject> CreateRecordBodyNote(ARROW_DIR dir, int _pos, int _holdTime, bool _charidx);

	TapIter FindSelectedTapNode(ImVec2 _pos, ImVec2 basePos, list<int>& _list);
	PressIter FindSelectedPressNode(ImVec2 _pos, ImVec2 basePos, list<pair<int, int>>& _list);

	float TimeToX_Normalized(int timeMs, int songLength);
	ImU32 GetArrowColor(int dir);

	void UpdateNoteActive();
	void UpdateNotePos();

private:
	// Record Related
	std::list<int> recordTapList[2][(int)ARROW_DIR::END];
	std::list<std::pair<int, int>> recordPressList[2][(int)ARROW_DIR::END];

	unordered_map<TapIter, RecordedTapNote, TapIterHash, TapIterEqual> m_recordedTapNotes;
	unordered_map<PressIter, RecordedPressNote, PressIterHash, PressIterEqual> m_recordedPressNotes;

	// Picking Related
	pair<TapIter, std::list<int>*> m_pickTapInfo;
	pair<PressIter, std::list<pair<int, int>>*> m_pickPressInfo;

	// ETC
	std::stack<pair<bool, int>> m_recentlyPos;		// charidx, pos
	int m_pressStart[4];

	const float m_drawWidth;
	const float m_drawHeight;

	float m_noteSpeed;
	float m_rectLength;		// 그려질 Note의 Node 크기

	DIFFICULTIES m_currentDiff;
};

