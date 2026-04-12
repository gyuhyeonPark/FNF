#include "pch.h"
#include "AudioWaveUI.h"
#include "ListUI.h"
#include "EditorManager.h"
#include "SoundManager.h"
#include "SongManager.h"
#include <fstream>
#include <rapidjson/prettywriter.h>
#include "NoteDataUI.h"
#include "CameraMoveDataUI.h"
#include "LightDataUI.h"

#include <commdlg.h>

using namespace rapidjson;

#define DRAW_WIDTH	1800.f
#define DRAW_HEIGHT 80.f
#define RECT_LENGTH 4.f
#define LOCAL_NOTESPEED 0.5f

float TimeToX_Normalized(int timeMs, int songLength)
{
	return (timeMs / (float)songLength) * DRAW_WIDTH;
}

float TimeToX(int timeMs)
{
	return timeMs * LOCAL_NOTESPEED;
}

AudioWaveUI::AudioWaveUI()
	: EditorUI("AudioWaveUI"), m_currentSong(STAGENUM::BOPEEBO),
	m_currentDiff(DIFFICULTIES::HARD),
	m_currentPos(0.f), m_sliderPos(0.f), m_recordMode(false), m_currentType(EDIT_TYPE::NOTE)
{
	AudioWaveNameVec.resize(STAGENUM::STAGEEND);

	AudioWaveNameVec[STAGENUM::BOPEEBO] = { "bopeebo\\Voice-bf.dat" ,"bopeebo\\Voice-dad.dat" };
	AudioWaveNameVec[STAGENUM::FRESH] = { "fresh\\Voice-bf.dat" ,"fresh\\Voice-dad.dat" };
	AudioWaveNameVec[STAGENUM::DADBATTLE] = { "dadbattle\\Voice-bf.dat" ,"dadbattle\\Voice-dad.dat" };
	AudioWaveNameVec[STAGENUM::SQUARE] = { "square\\Voice-bf.dat" ,"square\\Voice-dad.dat" };
	AudioWaveNameVec[STAGENUM::WHIPLASH] = { "whiplash\\Voice-bf.dat" ,"whiplash\\Voice-dad.dat" };
	AudioWaveNameVec[STAGENUM::LTR] = { "LTR\\Voice-bf.dat" ,"LTR\\Voice-dad.dat" };

	LoadWaveform(AudioWaveNameVec[STAGENUM::BOPEEBO].first, 0);
	LoadWaveform(AudioWaveNameVec[STAGENUM::BOPEEBO].second, 1);

	m_currentSong = STAGENUM::BOPEEBO;
	SongManager::GetInstance()->ChangeSong(m_currentSong);
	SOUNDMANAGER->CreateResources(SONGNUM);
	SOUNDMANAGER->ChangeStage(m_currentSong, false);

	m_songDataUIs[(UINT)EDIT_TYPE::NOTE] = new NoteDataUI;
	m_songDataUIs[(UINT)EDIT_TYPE::CAMERA] = new CameraMoveDataUI;
	m_songDataUIs[(UINT)EDIT_TYPE::LIGHT] = new LightDataUI;
}

AudioWaveUI::~AudioWaveUI()
{
	for (int i = 0; i < (UINT)EDIT_TYPE::END; ++i)
	{
		delete(m_songDataUIs[i]);
	}
}

void AudioWaveUI::Tick_UI()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::Button(GetUIName().c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	// Select Songe
	if (ImGui::Button("SONG", Vec2(80.f, 20.f)))
	{
		// 버튼이 눌리면, 리스트UI 를 찾아서 활성화 시키고, 출력시키고 싶은 문자열을 ListUI 에 등록시킨다.
		Ptr<ListUI> pUI = dynamic_cast<ListUI*>(EditorManager::GetInstance()->FindUI("ListUI").Get());
		assert(pUI.Get());

		pUI->SetUIName("Song List");

		vector<wstring> vecSongName;
		vecSongName = SongManager::GetInstance()->GetSongVec();
		pUI->AddString(vecSongName);
		pUI->AddDelegate(this, (DELEGATE_1)&AudioWaveUI::ChangeSong);
		pUI->SetActive(true);
	}

	ImGui::Separator();

	ImGui::Text("Player");

	// 여기에 wave 출력
	playerPos = ImGui::GetCursorScreenPos();
	DrawWaveform(0);

	ImGui::Text("Opponent");
	
	opponentPos = ImGui::GetCursorScreenPos();
	DrawWaveform(1);

	ImGui::Spacing();
	ImGui::PushItemWidth(DRAW_WIDTH);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(DRAW_WIDTH, 2)); // y값 ↑ → height 증가

	if (SOUNDMANAGER->IsPlaying())
	{
		int songLength = SoundManager::GetInstance()->GetTotalLength(SONGNUM);
		m_currentPos = SOUNDMANAGER->GetPosition();
		m_sliderPos = TimeToX_Normalized(m_currentPos, SOUNDMANAGER->GetTotalLength(SONGNUM));
	}

	if (ImGui::SliderInt("##/ms", &m_sliderPos, 0, (int)DRAW_WIDTH))
	{
		int songLength = SOUNDMANAGER->GetTotalLength(SONGNUM);

		// ✅ 픽셀 → 시간(ms)
		int result = (int)((m_sliderPos / (float)DRAW_WIDTH) * songLength);

		SOUNDMANAGER->SetPosition(result);

		// 🔥 이것도 같이 갱신 (중요)
		m_currentPos = result;
	}

	ImGui::PopItemWidth();
	ImGui::PopStyleVar();

	ImGui::Text("Music");

	if (ImGui::Button("PLAY", Vec2(80.f, 20.f)))
	{
		SoundManager::GetInstance()->Resume();

		int songLength = SoundManager::GetInstance()->GetTotalLength(SONGNUM);

		// 0 ~ width(1800) -> 0 ~ songLength 
		int result = (m_currentPos / DRAW_WIDTH) * songLength;

		SoundManager::GetInstance()->SetPosition(result);
	}

	ImGui::SameLine();
	if (ImGui::Button("PAUSE", Vec2(80.f, 20.f)))
	{
		SoundManager::GetInstance()->Pause();
	}

	ImGui::Separator();

	ImGui::Text("Mode");

	const char* eventLabel[] = { "NOTE", "CAMERA", "LIGHT", "ZOOM" };

	static int selected_idx = 0;
	const char* previewVal = eventLabel[selected_idx];


	if (ImGui::BeginCombo("Edit Type", previewVal, 0))
	{
		for (int n = 0; n < IM_COUNTOF(eventLabel); n++)
		{
			const bool is_selected = (selected_idx == n);
			if (ImGui::Selectable(eventLabel[n], is_selected))
			{
				selected_idx = n;
				m_currentType = (EDIT_TYPE)selected_idx;
				m_recordMode = false;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();

	if (ImGui::Button("SAVE", Vec2(80.f, 20.f)))
	{
		SaveMap();
	}

	// 기존 File Load
	if (ImGui::Button("Load Map"))
	{
		std::wstring path = OpenFileDialog();
		if (!path.empty())
		{
			LoadMap(path);
		}
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();

	// 현재 EditType에게 상시로 전달.
	m_songDataUIs[(UINT)m_currentType]->SetCharacterPos(playerPos, opponentPos);
	m_songDataUIs[(UINT)m_currentType]->SetCurrentPos(m_currentPos);
	m_songDataUIs[(UINT)m_currentType]->Update();
}

bool AudioWaveUI::LoadWaveform(const std::string& songName, int _idx)
{
	std::string p = "Resources\\Sounds\\BGM\\waveforms\\";
	std::wstring c = CONTENT_PATH;

	std::string cstr(c.begin(), c.end());

	std::string path = cstr + p + songName;

	std::ifstream file(path, std::ios::binary);
	if (!file)
		return false;

	m_waveform[_idx].clear();

	while (!file.eof())
	{
		WaveSample s;
		file.read((char*)&s, sizeof(WaveSample));

		if (file.gcount() == sizeof(WaveSample))
			m_waveform[_idx].push_back(s);
	}

	return true;
}

void AudioWaveUI::SaveMap()
{
	Document doc;
	doc.SetObject();
	auto& alloc = doc.GetAllocator();

	int songLength = SOUNDMANAGER->GetTotalLength(SONGNUM);

	// =========================
	// version
	// =========================
	doc.AddMember("version", "2.0.0", alloc);

	// =========================
	// scrollSpeed
	// =========================
	{
		Value scroll(kObjectType);
		scroll.AddMember("easy", 1.2f, alloc);
		scroll.AddMember("normal", 1.3f, alloc);
		scroll.AddMember("hard", 1.6f, alloc);
		doc.AddMember("scrollSpeed", scroll, alloc);
	}

	// =========================
	// events
	// =========================

	for (int i = 0; i < (UINT)EDIT_TYPE::END; ++i)
	{
		m_songDataUIs[i]->Save(doc);
	}

	// =========================
	// generatedBy
	// =========================
	doc.AddMember("generatedBy",
		"Custom Editor (ImGui + FMOD)",
		alloc);

	// =========================
	// 파일 저장
	// =========================
	wstring tempPath = CONTENT_PATH;
	tempPath += L"Resources\\SongData\\Custom\\" + SONGNAME + L".json";

	// ✅ wstring → UTF-8 string 변환
	int size = WideCharToMultiByte(
		CP_UTF8, 0,
		tempPath.c_str(), -1,
		nullptr, 0,
		nullptr, nullptr
	);

	std::string path(size - 1, 0);

	WideCharToMultiByte(
		CP_UTF8, 0,
		tempPath.c_str(), -1,
		&path[0], size,
		nullptr, nullptr
	);

	// JSON 생성
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	doc.Accept(writer);

	// ✅ 변환된 경로 사용
	std::ofstream file(path);
	file << buffer.GetString();
	file.close();
}

void AudioWaveUI::LoadMap(const wstring& fileName)
{
	using namespace rapidjson;

	// =========================
	// 경로 생성
	// =========================
	std::wstring tempPath = fileName;

	int size = WideCharToMultiByte(
		CP_UTF8, 0,
		tempPath.c_str(), -1,
		nullptr, 0,
		nullptr, nullptr
	);

	std::string path(size - 1, 0);

	WideCharToMultiByte(
		CP_UTF8, 0,
		tempPath.c_str(), -1,
		&path[0], size,
		nullptr, nullptr
	);

	// =========================
	// 파일 읽기
	// =========================
	std::ifstream file(path);
	if (!file.is_open())
		return;

	std::stringstream ss;
	ss << file.rdbuf();
	std::string jsonStr = ss.str();

	Document doc;
	doc.Parse(jsonStr.c_str());

	if (!doc.IsObject())
		return;

	for (int i = 0; i < (UINT)EDIT_TYPE::END; ++i)
	{
		m_songDataUIs[i]->ClearList();
		m_songDataUIs[i]->Load(doc);
	}
}

wstring AudioWaveUI::OpenFileDialog()
{
	wchar_t fileName[MAX_PATH] = L"";

	OPENFILENAMEW ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = L"All Files\0*.*\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (GetOpenFileNameW(&ofn))
		return fileName;

	return L"";
}

void AudioWaveUI::DrawWaveform(int _idx)
{
	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	int count = m_waveform[_idx].size();
	if (count == 0) return;

	int songLength = SOUNDMANAGER->GetTotalLength(SONGNUM);
	if (songLength <= 0) return;

	float step = DRAW_WIDTH / (float)count;

	// =====================
	// 1. Waveform
	// =====================
	for (int i = 0; i < count; i++)
	{
		float x = pos.x + i * step;

		float min = m_waveform[_idx][i].min / 128.0f;
		float max = m_waveform[_idx][i].max / 128.0f;

		float y1 = pos.y + DRAW_HEIGHT * (0.5f - max * 0.5f);
		float y2 = pos.y + DRAW_HEIGHT * (0.5f - min * 0.5f);

		draw->AddLine(ImVec2(x, y1), ImVec2(x, y2), IM_COL32(255, 255, 255, 255));
	}

	// =====================
	// 2. Note
	// =====================
	
	m_songDataUIs[(UINT)m_currentType]->DrawOnWaveForm(_idx);

	ImGui::Dummy(ImVec2(DRAW_WIDTH, DRAW_HEIGHT));

	// =====================
	// 3. Playhead
	// =====================
	{
		float totalTop = playerPos.y;
		float totalBottom = opponentPos.y + 80.f;

		float x = playerPos.x + TimeToX_Normalized((int)m_currentPos, songLength);

		draw->AddLine(
			ImVec2(x, totalTop),
			ImVec2(x, totalBottom),
			IM_COL32(66, 150, 250, 255),
			2.0f
		);
	}
}

void AudioWaveUI::ChangeSong(DWORD_PTR _ListUI)
{
	Ptr<ListUI> pListUI = ((ListUI*)_ListUI);

	wstring key = wstring(pListUI->GetSelectedString().begin(), pListUI->GetSelectedString().end());

	const vector<wstring>& songVec = SongManager::GetInstance()->GetSongVec();

	for (size_t i = 0; i < STAGENUM::STAGEEND; ++i)
	{
		if (songVec[i] == key)
		{
			if (m_currentSong == (STAGENUM)i)
				return;

			m_currentSong = (STAGENUM)i;
			SongManager::GetInstance()->ChangeSong(m_currentSong);
			SOUNDMANAGER->CreateResources(SONGNUM);
			SOUNDMANAGER->ChangeStage(m_currentSong, false);
			break;
		}
	}

	LoadWaveform(AudioWaveNameVec[m_currentSong].first, 0);
	LoadWaveform(AudioWaveNameVec[m_currentSong].second, 1);

	for (int i = 0; i < (UINT)EDIT_TYPE::END; ++i)
	{
		m_songDataUIs[i]->ClearList();
	}
}





