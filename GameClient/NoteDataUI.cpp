#include "pch.h"
#include "NoteDataUI.h"
#include "AssetKeys.h"
#include "AssetManager.h"
#include "Source\Scripts\CNote.h"
#include "SceneManager.h"
#include "Source\Scripts\CGameManager.h"

NoteDataUI::NoteDataUI()
	: EditorUI("NoteDataUI"),
	m_pressStart{}, m_charIdx(-1),
	m_drawWidth(1800.f), m_drawHeight(80.f),
	m_noteSpeed(0.5f), m_rectLength(4.f), m_currentDiff(DIFFICULTIES::HARD)
{
	m_pickTapInfo = { recordTapList[0][0].end(), nullptr };
	m_pickPressInfo = { recordPressList[0][0].end(), nullptr };
}

NoteDataUI::~NoteDataUI()
{
}

void NoteDataUI::Update()
{
	UpdateNoteActive();
	UpdateNotePos();
}

void NoteDataUI::DrawOnWaveForm(int _idx)
{
	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	int songLength = SOUNDMANAGER->GetTotalLength(SONGNUM);
	if (songLength <= 0) return;

	for (int type = 0; type < 2; type++)
	{
		if (type != _idx) continue;

		for (int dir = 0; dir < (int)ARROW_DIR::END; dir++)
		{
			auto& tapList = recordTapList[type][dir];
			auto& pressList = recordPressList[type][dir];

			// ===== Tap =====
			for (auto iter = tapList.begin(); iter != tapList.end(); ++iter)
			{
				ImU32 col = GetArrowColor(dir);

				if (&tapList == m_pickTapInfo.second && iter == m_pickTapInfo.first)
					col = IM_COL32(255, 230, 80, 255);

				int time = *iter;

				float x = pos.x + TimeToX_Normalized(time, songLength);
				float y = pos.y + m_drawHeight * 0.3f + dir * 10.f;

				draw->AddRectFilled(
					ImVec2(x - m_rectLength, y - m_rectLength),
					ImVec2(x + m_rectLength, y + m_rectLength),
					col
				);
			}

			// ===== Press =====
			for (auto iter = pressList.begin(); iter != pressList.end(); ++iter)
			{
				ImU32 col = GetArrowColor(dir);

				if (&pressList == m_pickPressInfo.second && iter == m_pickPressInfo.first)
					col = IM_COL32(255, 230, 80, 255);

				float start = (float)iter->first;
				float duration = (float)iter->second;

				float xStart = TimeToX_Normalized(start, songLength);
				float xEnd = TimeToX_Normalized(start + duration, songLength);

				float x = pos.x + (xStart + xEnd) * 0.5f;
				float width = (xEnd - xStart);

				float y = pos.y + m_drawHeight * 0.3f + dir * 10.f;

				draw->AddRectFilled(
					ImVec2(x - width / 2, y - m_rectLength),
					ImVec2(x + width / 2, y + m_rectLength),
					col
				);
			}
		}
	}
}

void NoteDataUI::Record()
{
	if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, false))
	{
		InsertTapNode(ARROW_DIR::LEFT);
	}
	if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false))
	{
		InsertTapNode(ARROW_DIR::DOWN);
	}
	if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, false))
	{
		InsertTapNode(ARROW_DIR::UP);
	}
	if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, false))
	{
		InsertTapNode(ARROW_DIR::RIGHT);
	}


	/// Press Start
	if (ImGui::IsKeyPressed(ImGuiKey_A, false))
	{
		m_pressStart[(int)ARROW_DIR::LEFT] = m_currentPos;
		m_recentlyPos.push({ m_charIdx, m_currentPos });
	}
	if (ImGui::IsKeyPressed(ImGuiKey_S, false))
	{
		m_pressStart[(int)ARROW_DIR::DOWN] = m_currentPos;
		m_recentlyPos.push({ m_charIdx, m_currentPos });
	}
	if (ImGui::IsKeyPressed(ImGuiKey_W, false))
	{
		m_pressStart[(int)ARROW_DIR::UP] = m_currentPos;
		m_recentlyPos.push({ m_charIdx, m_currentPos });
	}
	if (ImGui::IsKeyPressed(ImGuiKey_D, false))
	{
		m_pressStart[(int)ARROW_DIR::RIGHT] = m_currentPos;
		m_recentlyPos.push({ m_charIdx, m_currentPos });
	}

	// Press End
	if (ImGui::IsKeyReleased(ImGuiKey_A))
	{
		InsertPressNode(ARROW_DIR::LEFT);
	}
	if (ImGui::IsKeyReleased(ImGuiKey_S))
	{
		InsertPressNode(ARROW_DIR::DOWN);
	}
	if (ImGui::IsKeyReleased(ImGuiKey_W))
	{
		InsertPressNode(ARROW_DIR::UP);
	}
	if (ImGui::IsKeyReleased(ImGuiKey_D))
	{
		InsertPressNode(ARROW_DIR::RIGHT);
	}
}

void NoteDataUI::Undo()
{
	if (m_recentlyPos.empty())
		return;

	bool charIdx = m_recentlyPos.top().first;
	int deletePos = m_recentlyPos.top().second;
	m_recentlyPos.pop();

	int idx = -1;

	for (int i = 0; i < (int)ARROW_DIR::END; ++i)
	{
		list<int>& currentList = recordTapList[(int)charIdx][i];
		for (auto iter = currentList.begin(); iter != currentList.end(); )
		{
			if (*iter == deletePos)
			{
				m_recordedTapNotes[iter].obj->Destroy();
				m_recordedTapNotes.erase(iter);
				currentList.erase(iter);
				return;
			}
			else
				++iter;
		}
	}

	for (int i = 0; i < (int)ARROW_DIR::END; ++i)
	{
		list<pair<int, int>>& currentList = recordPressList[(int)charIdx][i];
		for (auto iter = currentList.begin(); iter != currentList.end(); )
		{
			if (iter->first == deletePos)
			{
				for (int i = 0; i < 3; ++i)
				{
					m_recordedPressNotes[iter].obj[i]->Destroy();
				}
				m_recordedPressNotes.erase(iter);
				currentList.erase(iter);
				return;
			}
			else
				++iter;
		}
	}

	// 여기까지 왔다는 건 중간에 삭제된 노드이기 때문이다. 다시 undo를 호출하자
	Undo();
}

void NoteDataUI::Delete()
{
	if (m_pickTapInfo.second != nullptr && !m_pickTapInfo.second->empty())
	{
		m_recordedTapNotes[m_pickTapInfo.first].obj->Destroy();
		m_recordedTapNotes.erase(m_pickTapInfo.first);
		m_pickTapInfo.second->erase(m_pickTapInfo.first);

		// 초기화
		m_pickTapInfo.first = m_pickTapInfo.second->end();
		m_pickTapInfo.second = nullptr;
	}
	else if (m_pickPressInfo.second != nullptr && !m_pickPressInfo.second->empty())
	{
		for (int i = 0; i < 3; ++i)
		{
			m_recordedPressNotes[m_pickPressInfo.first].obj[i]->Destroy();
		}
		m_recordedPressNotes.erase(m_pickPressInfo.first);
		m_pickPressInfo.second->erase(m_pickPressInfo.first);

		// 초기화
		m_pickPressInfo.first = m_pickPressInfo.second->end();
		m_pickPressInfo.second = nullptr;
	}
}

void NoteDataUI::ClearList()
{
	// 생성된 Note까지 전부 삭제해준다.
	for (auto& each : m_recordedTapNotes)
	{
		each.second.obj->Destroy();
	}
	m_recordedTapNotes.clear();

	for (auto& each : m_recordedPressNotes)
	{
		for (int i = 0; i < 3; ++i)
			each.second.obj[i]->Destroy();
	}
	m_recordedPressNotes.clear();

	for (UINT i = 0; i < (int)ARROW_DIR::END; ++i)
	{
		recordTapList[m_charIdx][i].clear();
		recordPressList[m_charIdx][i].clear();
	}
}

void NoteDataUI::Save(Document& doc)
{
	auto& alloc = doc.GetAllocator();

	// =========================
	// notes
	// =========================
	{
		Value notes(kObjectType);

		const char* diffNames[3] = { "easy", "normal", "hard" };

		for (int diff = 0; diff < 3; diff++)
		{
			Value arr(kArrayType);

			for (int type = 0; type < 2; type++)
			{
				for (int dir = 0; dir < (int)ARROW_DIR::END; dir++)
				{
					for (int timeMs : recordTapList[type][dir])
					{
						int lane = dir;
						if (type == 1) lane += 4;

						Value note(kObjectType);
						note.AddMember("t", timeMs, alloc);
						note.AddMember("d", lane, alloc);

						arr.PushBack(note, alloc);
					}

					for (pair<int, int> info : recordPressList[type][dir])
					{
						int timeMs = info.first;
						int lengthMs = info.second;

						int lane = dir;
						if (type == 1) lane += 4;

						Value note(kObjectType);
						note.AddMember("t", timeMs, alloc);
						note.AddMember("d", lane, alloc);
						note.AddMember("l", lengthMs, alloc);

						arr.PushBack(note, alloc);
					}
				}
			}

			// 정렬
			std::sort(arr.Begin(), arr.End(),
				[](const Value& a, const Value& b)
				{
					return a["t"].GetInt() < b["t"].GetInt();
				});

			notes.AddMember(Value().SetString(diffNames[diff], alloc), arr, alloc);
		}

		doc.AddMember("notes", notes, alloc);
	}
}

void NoteDataUI::Load(Document& doc)
{
	// =========================
	// 기존 데이터 초기화
	// =========================
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < (int)ARROW_DIR::END; ++j)
		{
			recordTapList[i][j].clear();
			recordPressList[i][j].clear();
		}
	}
	for (auto& each : m_recordedTapNotes)
	{
		each.second.obj->Destroy();
	}
	for (auto& each : m_recordedPressNotes)
	{
		for (int i = 0; i < 3; ++i)
			each.second.obj[i]->Destroy();
	}
	m_recordedTapNotes.clear();
	m_recordedPressNotes.clear();

	// =========================
	// notes 로드
	// =========================
	if (doc.HasMember("notes"))
	{
		const Value& notes = doc["notes"];

		const char* diffNames[3] = { "easy", "normal", "hard" };

		// 현재는 HARD만 사용한다고 가정
		const Value& arr = notes[diffNames[(int)m_currentDiff]];

		for (auto& n : arr.GetArray())
		{
			int timeMs = 0;
			int lane = 0;

			if (n["t"].IsInt())
				timeMs = n["t"].GetInt();
			if (n["d"].IsInt())
				lane = n["d"].GetInt();

			int type = (lane >= 4) ? 1 : 0;
			int dir = lane % 4;

			// sustain note
			if (n.HasMember("l") && n["l"].IsInt())
			{
				int lengthMs = n["l"].GetInt();

				recordPressList[type][dir].push_back({ timeMs, lengthMs });
			}
			else
			{
				recordTapList[type][dir].push_back(timeMs);
			}
		}
	}

	// 채워진 list 기반 note 생성 작업.
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < (UINT)ARROW_DIR::END; ++j)
		{
			{
				list<int>& pList = recordTapList[i][j];
				for (auto iter = pList.begin(); iter != pList.end(); ++iter)
				{
					Ptr<GameObject> noteObj = CreateRecordTapNote((ARROW_DIR)j, *iter, i);
					m_recordedTapNotes[iter] = { noteObj, *iter, (ARROW_DIR)j };
				}
			}
			{
				list<pair<int, int>>& pList = recordPressList[i][j];
				for (auto iter = pList.begin(); iter != pList.end(); ++iter)
				{
					int startPos = iter->first;
					int pixelLength = iter->second;
					ARROW_DIR dir = static_cast<ARROW_DIR>(j);

					Ptr<GameObject> headObj = CreateRecordTapNote(dir, startPos, i);
					Ptr<GameObject> bodyObj = CreateRecordBodyNote(dir, startPos, pixelLength, i);
					Ptr<GameObject> tailObj = CreateRecordTapNote(dir, startPos, i, false);

					m_recordedPressNotes[iter] = RecordedPressNote({ headObj , bodyObj , tailObj }, startPos, pixelLength, dir);
				}
			}
		}
	}
}

void NoteDataUI::DetectPicking()
{
	if (ImGui::IsMouseClicked(0))
	{
		ImVec2 mPos = ImGui::GetMousePos();

		std::list<int>* pSelectedTapList = nullptr;
		std::list<pair<int, int>>* pSelectedPressList = nullptr;

		// yPos에 따라 해당 위치에 있는 리스트를 선택.
		int selectedType = -1;
		int selectedDir = -1;

		// =========================
		// 1. 어느 waveform인지 판별
		// =========================
		ImVec2 basePos;

		if (mPos.y >= m_playerPos.y && mPos.y <= m_playerPos.y + m_drawHeight)
		{
			selectedType = 0;
			basePos = m_playerPos;
		}
		else if (mPos.y >= m_opponentPos.y && mPos.y <= m_opponentPos.y + m_drawHeight)
		{
			selectedType = 1;
			basePos = m_opponentPos;
		}

		// =========================
		// 2. lane(dir) 계산
		// =========================
		if (selectedType != -1)
		{
			float baseY = basePos.y + m_drawHeight * 0.3f;
			float relativeY = mPos.y - baseY;

			int dir = (int)(relativeY / 10.f);

			if (dir >= 0 && dir < (int)ARROW_DIR::END)
			{
				selectedDir = dir;

				pSelectedTapList = &recordTapList[selectedType][selectedDir];
				pSelectedPressList = &recordPressList[selectedType][selectedDir];
			}
		}


		// pSelectedTapList와 pSelectedPressList를 순회하면서 현재 마우스가 찍은 좌표와 비교하는 작업 필요.
		// 일단은 pSelectedTapList 우선.

		if (pSelectedTapList && !pSelectedTapList->empty())
		{
			const TapIter& selectedTapIter = FindSelectedTapNode(mPos, basePos, *pSelectedTapList);
			if (pSelectedTapList != nullptr && selectedTapIter != pSelectedTapList->end())
			{
				m_pickTapInfo = { selectedTapIter, pSelectedTapList };
				// press는 null 초기화
				m_pickPressInfo = {};
			}
		}
		if (pSelectedPressList && !pSelectedPressList->empty())
		{
			const PressIter& selectedPressIter = FindSelectedPressNode(mPos, basePos, *pSelectedPressList);
			if (pSelectedPressList != nullptr && selectedPressIter != pSelectedPressList->end())
			{
				m_pickPressInfo = { selectedPressIter, pSelectedPressList };
				// Tap은 null 초기화
				m_pickTapInfo = {};
			}
		}
	}
}

void NoteDataUI::InsertTapNode(ARROW_DIR dir)
{
	std::list<int>& pList = recordTapList[m_charIdx][(int)dir];
	TapIter inserted;

	float time = m_currentPos;


	if (pList.empty())
		inserted = pList.insert(pList.begin(), m_currentPos);
	else
	{
		TapIter insertIter = pList.end();

		for (auto iter = --pList.end(); ;)
		{
			if (iter == pList.begin())
			{
				inserted = pList.insert(pList.begin(), m_currentPos);
				break;
			}

			if (*iter < m_currentPos)
			{
				inserted = pList.insert(insertIter, m_currentPos);
				break;
			}
			else
			{
				insertIter = iter;
				--iter;
			}
		}
	}
	m_recentlyPos.push({ m_charIdx, m_currentPos });
	Ptr<GameObject> noteObj = CreateRecordTapNote(dir, m_currentPos, m_charIdx);

	m_recordedTapNotes.insert({ inserted, { noteObj, m_currentPos, dir } });
}

void NoteDataUI::InsertPressNode(ARROW_DIR dir)
{
	std::list<pair<int, int>>& pList = recordPressList[m_charIdx][(int)dir];
	PressIter inserted;

	int holdTimeSec = m_currentPos - m_pressStart[(int)dir];
	pair<int, int> val = { m_pressStart[(int)dir], holdTimeSec };

	if (pList.empty())
		inserted = pList.insert(pList.begin(), val);
	else
	{
		PressIter insertIter = pList.end();

		for (auto iter = --pList.end(); ;)
		{
			if (iter == pList.begin())
			{
				inserted = pList.insert(pList.begin(), val);
				break;
			}

			if (iter->first < m_currentPos)
			{
				inserted = pList.insert(insertIter, val);
				break;
			}
			else
			{
				inserted = iter;
				--iter;
			}
		}
	}
	Ptr<GameObject> headObj = CreateRecordTapNote(dir, m_currentPos, m_charIdx);
	Ptr<GameObject> bodyObj = CreateRecordBodyNote(dir, m_currentPos, holdTimeSec, m_charIdx);
	Ptr<GameObject> tailObj = CreateRecordTapNote(dir, m_currentPos, m_charIdx, false);

	// Note
	// 시간 차이 (pixel 기준)
	int holdTime = m_currentPos - m_pressStart[(int)dir];

	m_recordedPressNotes.insert({ inserted,
		{ { headObj , bodyObj , tailObj }, m_pressStart[(int)dir], holdTime, dir }
		});
}

Ptr<GameObject> NoteDataUI::CreateRecordTapNote(ARROW_DIR dir, int _pos, bool _charidx, bool isHead)
{
	std::list<int>& pList = recordTapList[_charidx][(int)dir];

	// Note
	float noteSpeed = 500.f;

	Ptr<ASprite>	sprite = new ASprite;
	wstring path;
	if (isHead)
		path = L"Sprite\\" + noteNames[(UINT)dir] + L".sprite";
	else
		path = L"Sprite\\" + noteHoldNames[(UINT)dir * 2 + 1] + L".sprite";

	sprite = LOAD(ASprite, path);     // body

	Ptr<CNote> noteCom = new CNote;
	noteCom->SetNoteSpeed(noteSpeed);
	noteCom->SetArrowDir(dir);
	Ptr<GameObject> noteObj = new GameObject;
	wchar_t Buff[50] = {};
	swprintf_s(Buff, L"Note_%d%d", (UINT)dir + _charidx * 4, pList.size());

	noteObj->SetName(Buff);
	noteObj->AddComponent(new CTransform);
	noteObj->AddComponent(new CSpriteRenderer);
	//noteObj->AddComponent(noteCom.Get());

	noteObj->SpriteRenderer()->SetSprite(sprite);
	noteObj->SpriteRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));

	noteObj->GetTransform()->SetIndependentScale(true);

	if (isHead)
		noteObj->GetTransform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	else
		noteObj->GetTransform()->SetRelativeScale(Vec3(35.f, 100.f, 1.f));

	int songLength = SOUNDMANAGER->GetTotalLength(SONGNUM);
	float pixel = (float)_pos / songLength * m_drawWidth;

	Vec3 pos = Vec3(0.f, -pixel * m_noteSpeed, -1.f);
	if (!isHead)
	{
		// 시간 차이 (pixel 기준)
		int holdTime = _pos - m_pressStart[(int)dir];

		// 👉 길이도 동일한 변환 적용
		float holdPixelLength = holdTime * m_noteSpeed;

		pos = Vec3(0.f, -_pos * m_noteSpeed + holdPixelLength * 0.5f, 1.f);
	}

	noteObj->GetTransform()->SetRelativePosition(pos);

	CGameManager* gm = SceneManager::GetInstance()->FindObjectByName(L"GM")
		->GetScript<CGameManager>().Get();
	gm->GetNoteReceptors()[(UINT)dir + _charidx * 4]->GetOwner()->AddChild(noteObj);

	return noteObj;
}

Ptr<GameObject> NoteDataUI::CreateRecordBodyNote(ARROW_DIR dir, int _pos, int _holdTime, bool _charidx)
{
	std::list<pair<int, int>>& pList = recordPressList[_charidx][(int)dir];

	// Note

	// 시간 차이 (pixel 기준)
	int holdTime = _holdTime;

	// 👉 길이도 동일한 변환 적용
	// ⭐ ms → pixel 변환
	int songLength = SOUNDMANAGER->GetTotalLength(SONGNUM);

	float startPixel = (float)_pos / songLength * m_drawWidth;
	float holdPixelLength = (float)_holdTime / songLength * m_drawWidth;

	// Note
	Ptr<ASprite>	sprite = new ASprite;
	Ptr<GameObject> barObj = new GameObject;

	wstring path = L"Sprite\\" + noteHoldNames[(UINT)dir * 2] + L".sprite";
	sprite = LOAD(ASprite, path);     // body

	barObj->AddComponent(new CTransform);
	barObj->AddComponent(new CSpriteRenderer);
	wchar_t Buff[50] = {};
	swprintf_s(Buff, L"Body_%d%d", (UINT)dir + _charidx * 4, pList.size());

	barObj->SetName(Buff);

	barObj->SpriteRenderer()->SetSprite(sprite);
	barObj->SpriteRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));

	barObj->GetTransform()->SetIndependentScale(true);

	float holdLength = _holdTime * m_noteSpeed;

	// 최소 길이 보정 (짧은 노트 안 보이는 문제 방지)
	holdLength = max(holdLength, 10.f);

	barObj->GetTransform()->SetRelativeScale(Vec3(35.f, holdLength, 1.f));

	float startPos = _pos * m_noteSpeed;

	Vec3 pos = Vec3(0.f, -startPos, -1.f);
	barObj->GetTransform()->SetRelativePosition(pos);

	CGameManager* gm = SceneManager::GetInstance()->FindObjectByName(L"GM")
		->GetScript<CGameManager>().Get();

	gm->GetNoteReceptors()[(UINT)dir + _charidx * 4]->GetOwner()->AddChild(barObj);

	return barObj;
}

TapIter NoteDataUI::FindSelectedTapNode(ImVec2 _pos, ImVec2 basePos, list<int>& _list)
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

PressIter NoteDataUI::FindSelectedPressNode(ImVec2 _pos, ImVec2 basePos, list<pair<int, int>>& _list)
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

float NoteDataUI::TimeToX_Normalized(int timeMs, int songLength)
{
	return (timeMs / (float)songLength) * m_drawWidth;
}

ImU32 NoteDataUI::GetArrowColor(int dir)
{
	switch (dir)
	{
	case 0: return IM_COL32(194, 75, 153, 255); // Left (보라)
	case 1: return IM_COL32(75, 194, 255, 255); // Down (파랑)
	case 2: return IM_COL32(75, 255, 128, 255); // Up (초록)
	case 3: return IM_COL32(255, 75, 75, 255);  // Right (빨강)
	default: return IM_COL32(255, 255, 255, 255);
	}
}

void NoteDataUI::UpdateNoteActive()
{
	// Note들 최적화
	for (auto& each : m_recordedTapNotes)
	{
		float yPos = each.second.obj->Transform()->GetWorldPos().y;
		if (yPos >= 700.f || yPos <= -500.f)
			each.second.obj->SetActive(false);
		else
			each.second.obj->SetActive(true);
	}
	for (auto& each : m_recordedPressNotes)
	{
		for (int i = 0; i < 3; ++i)
		{
			float yPos = each.second.obj[i]->Transform()->GetWorldPos().y;
			if (yPos >= 700.f || yPos <= -400.f)
				each.second.obj[i]->SetActive(false);
			else
				each.second.obj[i]->SetActive(true);
		}
	}
}

void NoteDataUI::UpdateNotePos()
{
	for (auto& each : m_recordedTapNotes)
	{
		GameObject* noteObj = each.second.obj.Get();
		int noteTime = each.second.time;

		// 현재 위치와의 차이
		float delta = (float)(noteTime - m_currentPos);

		// 부모(Receptor) 기준 상대 위치
		float y = -delta * m_noteSpeed;

		noteObj->Transform()->SetRelativePosition(Vec3(0.f, y, -1.f));
	}

	for (auto& each : m_recordedPressNotes)
	{
		float baseTime = (float)each.second.time;

		float holdTime = each.second.hold;
		float startTime = (float)each.second.time;

		for (int j = 0; j < 3; j++)
		{
			GameObject* noteObj = each.second.obj[j].Get();

			float noteTime;

			if (j == 0)          // head
				noteTime = startTime;
			else if (j == 1)     // body (중앙)
				noteTime = startTime + holdTime * 0.5f;
			else                 // tail
				noteTime = startTime + holdTime;

			float delta = noteTime - m_currentPos;
			float y = -delta * m_noteSpeed;

			noteObj->Transform()->SetRelativePosition(Vec3(0.f, y, -1.f));
		}
	}
}
