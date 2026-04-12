#include "pch.h"
#include "CameraMoveDataUI.h"
#include "SongManager.h"

CameraMoveDataUI::CameraMoveDataUI()
	: EditorUI("NoteDataUI")
{
	m_pickEventInfo = { eventTimings[0].end(), nullptr };
}

CameraMoveDataUI::~CameraMoveDataUI()
{
}

void CameraMoveDataUI::Update()
{
	ImGui::Text("Recording");

	const char* label = m_recordMode ? "STOPRECORD" : "STARTRECORD";

	if (ImGui::Button(label, Vec2(140.f, 20.f)))
	{
		m_recordMode = !m_recordMode;
	}

	if (m_recordMode)
		Record();

	if (ImGui::Button("Undo", Vec2(80.f, 20.f)))
	{
		Undo();
	}

	ImGui::SameLine();
	if (ImGui::Button("Clear", Vec2(80.f, 20.f)))
	{
		ClearList();
	}
	if (ImGui::IsKeyPressed(ImGuiKey_Delete, false))
	{
		Delete();
	}
}

void CameraMoveDataUI::DrawOnWaveForm(int _idx)
{
	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	int songLength = SOUNDMANAGER->GetTotalLength(SONGNUM);
	if (songLength <= 0) return;

	for (int type = 0; type < 2; type++)
	{
		if (type != _idx) continue;

		auto& eList = eventTimings[type];

		for (auto iter = eList.begin(); iter != eList.end(); ++iter)
		{
			int time = *iter;

			float x = pos.x + TimeToX_Normalized(time, songLength);
			float y = pos.y + m_drawHeight * 0.5f;

			ImU32 col = IM_COL32(255, 0, 0, 255);

			if (&eList == m_pickEventInfo.second && iter == m_pickEventInfo.first)
				col = IM_COL32(255, 230, 80, 255);

			draw->AddRectFilled(
				ImVec2(x - m_rectLength, y - m_rectLength),
				ImVec2(x + m_rectLength, y + m_rectLength),
				col
			);
		}
	}
}

void CameraMoveDataUI::ClearList()
{
	for (UINT i = 0; i < 2; ++i)
	{
		eventTimings[i].clear();
	}
}

void CameraMoveDataUI::Save(Document& doc)
{
	auto& alloc = doc.GetAllocator();

	// 🔥 1. 새 events 배열 생성
	rapidjson::Value events(rapidjson::kArrayType);

	// 🔥 2. 데이터 채우기
	for (int side = 0; side < 2; side++)
	{
		for (int timeMs : eventTimings[side])
		{
			rapidjson::Value e(rapidjson::kObjectType);

			// t (int)
			e.AddMember("t", timeMs, alloc);

			// e (string) - 안전하게 Value로 생성
			e.AddMember("e", rapidjson::Value("FocusCamera", alloc), alloc);

			// v (int)
			e.AddMember("v", side, alloc);

			events.PushBack(e, alloc);
		}
	}

	// 🔥 3. 기존 events 제거 후 교체
	if (doc.HasMember("events"))
		doc.RemoveMember("events");

	doc.AddMember("events", events, alloc);
}

void CameraMoveDataUI::Load(Document& doc)
{
	// 🔥 초기화
	for (int i = 0; i < 2; ++i)
		eventTimings[i].clear();

	if (!doc.HasMember("events") || !doc["events"].IsArray())
		return;

	const Value& events = doc["events"];

	for (auto& e : events.GetArray())
	{
		if (!e.HasMember("e") || !e["e"].IsString())
			continue;

		const char* eventName = e["e"].GetString();

		if (strcmp(eventName, "FocusCamera") != 0)
			continue;

		if (!e.HasMember("t") || !e["t"].IsInt())
			continue;

		int timeMs = e["t"].GetInt();

		int side = 0;
		if (e.HasMember("v") && e["v"].IsInt())
			side = e["v"].GetInt();

		if (side < 0 || side >= 2)
			continue;

		eventTimings[side].push_back(timeMs);
	}
}

void CameraMoveDataUI::Record()
{
	if (ImGui::Button("Player", Vec2(80.f, 20.f)))
	{
		eventTimings[0].push_back(m_currentPos);
		m_charIdx = 0;
	}
	ImGui::SameLine();

	if (ImGui::Button("Opponent", Vec2(80.f, 20.f)))
	{
		eventTimings[1].push_back(m_currentPos);
		m_charIdx = 1;
	}
}

void CameraMoveDataUI::Undo()
{
	int time = 0;
	int idx = -1;
	int curIdx = (int)m_charIdx;

	for (UINT i = 0; i < 2; ++i)
	{
		if (eventTimings[i].empty())
			continue;

		auto iter = --eventTimings[i].end();

		if (time < *(iter))
		{
			idx = i;
			time = *(iter);
		}
	}

	if (idx != -1)
		eventTimings[idx]
		.erase(--eventTimings[idx].end());
}

void CameraMoveDataUI::Delete()
{
	if (m_pickEventInfo.second != nullptr && !m_pickEventInfo.second->empty())
	{
		m_pickEventInfo.second->erase(m_pickEventInfo.first);

		// 초기화
		m_pickEventInfo.first = m_pickEventInfo.second->end();
		m_pickEventInfo.second = nullptr;
	}
}

void CameraMoveDataUI::DetectPicking()
{
	if (ImGui::IsMouseClicked(0))
	{
		ImVec2 mPos = ImGui::GetMousePos();

		int selectedType = -1;
		std::list<int>* pEventList = nullptr;

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

		pEventList = &eventTimings[selectedType];

		if (pEventList != nullptr && !pEventList->empty())
		{
			const TapIter& selectedEventIter = FindSelectedTapNode(mPos, basePos, *pEventList);
			m_pickEventInfo = { selectedEventIter, pEventList };
		}
	}
}
