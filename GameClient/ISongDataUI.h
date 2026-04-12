#pragma once
#include "EditorUI.h"
#include <rapidjson/prettywriter.h>

using namespace rapidjson;

enum class EDIT_TYPE
{
	NOTE,
	CAMERA,
	LIGHT,
	ZOOM,
	END,
};

class ISongDataUI : virtual public EditorUI
{
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

protected:
	bool m_charIdx;		// 0 : player  1 : opponent

	int m_currentPos;
	bool m_recordMode;

	// WaveForm 상에서의 좌표
	ImVec2 m_playerPos;
	ImVec2 m_opponentPos;
};

