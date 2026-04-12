#include "pch.h"
#include "LightDataUI.h"
#include "SongManager.h"

LightDataUI::LightDataUI()
	: EditorUI("LightDataUI"), ISongDataUI()
{
	m_pickEventInfo = { lightTimings[0].end(), nullptr };
	m_currentRecordingInfo.first = -1;
}


LightDataUI::~LightDataUI()
{
}

void LightDataUI::Update()
{
	ImGui::Text("Recording");

	if (ImGui::Button("Player", Vec2(80.f, 20.f)))
	{
		m_charIdx = 0;
	}
	ImGui::SameLine();

	if (ImGui::Button("Opponent", Vec2(80.f, 20.f)))
	{
		m_charIdx = 1;
	}

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

void LightDataUI::DrawOnWaveForm(int _idx)
{
	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	int songLength = SOUNDMANAGER->GetTotalLength(SONGNUM);
	if (songLength <= 0) return;

	for (int type = 0; type < 2; type++)
	{
		if (type != _idx) continue;

		auto& pList = lightTimings[type];

		// ===== Press =====
		for (auto iter = pList.begin(); iter != pList.end(); ++iter)
		{
			ImU32 col = IM_COL32(255, 0, 0, 255);

			if (&pList == m_pickEventInfo.second && iter == m_pickEventInfo.first)
				col = IM_COL32(255, 230, 80, 255);

			float start = (float)iter->first;
			float end = (float)iter->second;

			float xStart = TimeToX_Normalized(start, songLength);
			float xEnd = TimeToX_Normalized(end, songLength);

			float x = pos.x + (xStart + xEnd) * 0.5f;
			float width = (xEnd - xStart);

			float y = pos.y + m_drawHeight * 0.5f;

			draw->AddRectFilled(
				ImVec2(x - width / 2, y - m_rectLength),
				ImVec2(x + width / 2, y + m_rectLength),
				col
			);
		}
	}
}

void LightDataUI::ClearList()
{
	for (int i = 0; i < 2; ++i)
		lightTimings[i].clear();
}

void LightDataUI::Save(Document& doc)
{
	auto& alloc = doc.GetAllocator();

	if (!doc.HasMember("events"))
	{
		Value events(kArrayType);
		doc.AddMember("events", events, alloc);
	}

	auto& events = doc["events"];

	for (int side = 0; side < 2; side++)
	{
		for (auto& timing : lightTimings[side])
		{
			Value e(kObjectType);

			e.AddMember("t", timing.first, alloc);
			e.AddMember("e", "Light", alloc);

			Value v(kObjectType);
			v.AddMember("side", side, alloc);
			v.AddMember("end", timing.second, alloc);

			e.AddMember("v", v, alloc);

			events.PushBack(e, alloc);
		}
	}
}

void LightDataUI::Load(Document& doc)
{
	// 🔥 초기화
	for (int i = 0; i < 2; ++i)
		lightTimings[i].clear();

	if (!doc.HasMember("events") || !doc["events"].IsArray())
		return;

	const Value& events = doc["events"];

	for (auto& e : events.GetArray())
	{
		if (!e.HasMember("e") || !e["e"].IsString())
			continue;

		const char* eventName = e["e"].GetString();

		if (strcmp(eventName, "Light") != 0)
			continue;

		if (!e.HasMember("t") || !e["t"].IsInt())
			continue;

		int start = e["t"].GetInt();

		if (!e.HasMember("v") || !e["v"].IsObject())
			continue;

		const Value& v = e["v"];

		int side = 0;
		int end = start;

		if (v.HasMember("side") && v["side"].IsInt())
			side = v["side"].GetInt();

		if (v.HasMember("end") && v["end"].IsInt())
			end = v["end"].GetInt();

		if (side < 0 || side >= 2)
			continue;

		lightTimings[side].push_back({ start, end });
	}
}

void LightDataUI::Record()
{
	if (ImGui::Button("Start", Vec2(80.f, 20.f)))
	{
		if (m_currentRecordingInfo.first == -1)
			m_currentRecordingInfo.first = m_currentPos;
	}
	ImGui::SameLine();

	if (ImGui::Button("End", Vec2(80.f, 20.f)))
	{
		if (m_currentRecordingInfo.first < m_currentPos && m_currentRecordingInfo.first != -1)
		{
			m_currentRecordingInfo.second = m_currentPos;
			lightTimings[m_charIdx].push_back(m_currentRecordingInfo);
			m_currentRecordingInfo.first = -1;
		}
	}
}

void LightDataUI::Undo()
{
	int time = 0;
	int idx = -1;
	int curIdx = (int)m_charIdx;

	for (UINT i = 0; i < 2; ++i)
	{
		if (lightTimings[i].empty())
			continue;

		auto iter = --lightTimings[i].end();

		if (time < (*iter).first)
		{
			idx = i;
			time = (*iter).first;
		}
	}

	if (idx != -1)
		lightTimings[idx]
		.erase(--lightTimings[idx].end());
}

void LightDataUI::Delete()
{
	if (m_pickEventInfo.second != nullptr && !m_pickEventInfo.second->empty())
	{
		m_pickEventInfo.second->erase(m_pickEventInfo.first);

		// 초기화
		m_pickEventInfo.first = m_pickEventInfo.second->end();
		m_pickEventInfo.second = nullptr;
	}
}

void LightDataUI::DetectPicking()
{
	if (ImGui::IsMouseClicked(0))
	{
		ImVec2 mPos = ImGui::GetMousePos();

		int selectedType = -1;
		std::list<pair<int,int>>* pEventList = nullptr;

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

		pEventList = &lightTimings[selectedType];

		if (pEventList != nullptr && !pEventList->empty())
		{
			const PressIter& selectedEventIter = FindSelectedPressNode(mPos, basePos, *pEventList);
			m_pickEventInfo = { selectedEventIter, pEventList };
		}
	}
}
