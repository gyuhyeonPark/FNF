#pragma once
#include "EditorUI.h"
#include <rapidjson/prettywriter.h>

using namespace rapidjson;

enum class EDIT_TYPE
{
	NOTE,
	CAMERA,
	LIGHT,
	//ZOOM,
	END,
};

typedef std::list<int>::iterator TapIter;
typedef std::list<std::pair<int, int>>::iterator PressIter;

class ISongDataUI : virtual public EditorUI
{
public:
	ISongDataUI();
	virtual ~ISongDataUI();

public:
	virtual void Update() = 0;
	virtual void DrawOnWaveForm(int _idx) = 0;
	virtual void ClearList() = 0;

	virtual void Save(Document& doc) = 0;
	virtual void Load(Document& doc) = 0;

public:
	virtual void SetCurrentPos(int _pos) { m_currentPos = _pos; }
	virtual void SetCharacterPos(ImVec2 _pPos, ImVec2 _oPos) { m_playerPos = _pPos; m_opponentPos = _oPos; }

protected:
	virtual void Record() = 0;
	virtual void Undo() = 0;
	virtual void Delete() = 0;
	virtual void DetectPicking() = 0;

	float TimeToX_Normalized(int timeMs, int songLength)
	{
		return (timeMs / (float)songLength) * m_drawWidth;
	}

	TapIter FindSelectedTapNode(ImVec2 _pos, ImVec2 basePos, list<int>& _list);
	PressIter FindSelectedPressNode(ImVec2 _pos, ImVec2 basePos, list<pair<int, int>>& _list);


protected:
	bool m_charIdx;		// 0 : player  1 : opponent

	int m_currentPos;
	bool m_recordMode;

	// WaveForm 상에서의 좌표
	ImVec2 m_playerPos;
	ImVec2 m_opponentPos;

	const float m_drawWidth;
	const float m_drawHeight;
	float m_rectLength;		// 그려질 Note의 Node 크기
};

