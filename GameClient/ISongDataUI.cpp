#include "pch.h"
#include "ISongDataUI.h"
#include "SongManager.h"

ISongDataUI::ISongDataUI()
	: m_drawWidth(1800.f), m_drawHeight(80.f), m_rectLength(4.f)
{
}

ISongDataUI::~ISongDataUI()
{
}

TapIter ISongDataUI::FindSelectedTapNode(ImVec2 _pos, ImVec2 basePos, list<int>& _list)
{
	TapIter returnIter = _list.end();

	int songLength = SOUNDMANAGER->GetTotalLength(SONGNUM);

	for (auto iter = _list.begin(); iter != _list.end(); ++iter)
	{
		float x = basePos.x + TimeToX_Normalized(*iter, songLength);

		if (_pos.x < x + m_rectLength && _pos.x > x - m_rectLength)
		{
			returnIter = iter;
			break;
		}
	}

	return returnIter;
}

PressIter ISongDataUI::FindSelectedPressNode(ImVec2 _pos, ImVec2 basePos, list<pair<int, int>>& _list)
{
	PressIter returnIter = _list.end();

	int songLength = SOUNDMANAGER->GetTotalLength(SONGNUM);

	for (auto iter = _list.begin(); iter != _list.end(); ++iter)
	{
		float start = (float)iter->first;
		float duration = (float)iter->second;

		float xStart = TimeToX_Normalized(start, songLength);
		float xEnd = TimeToX_Normalized(start + duration, songLength);

		float xMin = basePos.x + xStart;
		float xMax = basePos.x + xEnd;

		if (_pos.x >= xMin && _pos.x <= xMax)
		{
			returnIter = iter;
			break;
		}
	}

	return returnIter;
}
