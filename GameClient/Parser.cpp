#include "pch.h"
#include "Parser.h"
#include "Source\Scripts\CNoteReceptor.h"
#include "Source\Scripts\CEventManager.h"

Parser::Parser()
{
    wstring p = PathManager::GetInstance()->GetContentsPath();
    string xmlPath = std::string().assign(p.begin(), p.end());
    m_path = xmlPath + "Resources\\";
}

bool Parser::LoadTextureAtlas(const std::string& xmlName, int atlasWidth, int atlasHeight, std::unordered_map<std::string,
    AtlasAnimation>& outAnimations)
{
    string xmlPath = xmlName;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(xmlPath.c_str()) != XML_SUCCESS)
        return false;

    XMLElement* root = doc.FirstChildElement("TextureAtlas");
    if (!root) return false;

    for (XMLElement* elem = root->FirstChildElement("SubTexture");
        elem;
        elem = elem->NextSiblingElement("SubTexture"))
    {
        AtlasFrame frame;
        frame.name = elem->Attribute("name");

        elem->QueryIntAttribute("x", &frame.x);
        elem->QueryIntAttribute("y", &frame.y);
        elem->QueryIntAttribute("width", &frame.width);
        elem->QueryIntAttribute("height", &frame.height);

        elem->QueryIntAttribute("frameX", &frame.frameX);
        elem->QueryIntAttribute("frameY", &frame.frameY);
        elem->QueryIntAttribute("frameWidth", &frame.frameWidth);
        elem->QueryIntAttribute("frameHeight", &frame.frameHeight);

        frame.rotated = elem->BoolAttribute("rotated", false);

        // UV 계산
        frame.u0 = frame.x / (float)atlasWidth;
        frame.v0 = frame.y / (float)atlasHeight;
        frame.u1 = (frame.x + frame.width) / (float)atlasWidth;
        frame.v1 = (frame.y + frame.height) / (float)atlasHeight;
        
        // 애니메이션 이름 분리 (뒤 숫자 제거)
        std::string animName = frame.name;
        animName.erase(
            std::find_if(animName.rbegin(), animName.rend(),
                [](char c) { return !isdigit(c); }).base(),
            animName.end()
        );

        outAnimations[animName].frames.push_back(frame);
    }

    return true;
}

void Parser::BuildUV(const AtlasFrame& f, Vec2 uv[4])
{
    if (!f.rotated)
    {
        uv[0] = Vec2{ f.u0, f.v0 };
        uv[1] = Vec2{ f.u1, f.v0 };
        uv[2] = Vec2{ f.u0, f.v1 };
        uv[3] = Vec2{ f.u1, f.v1 };
    }
    else
    {
        // 90도 시계 회전 기준 (TexturePacker / Starling 방식)
        uv[0] = Vec2{ f.u1, f.v0 };
        uv[1] = Vec2{ f.u1, f.v1 };
        uv[2] = Vec2{ f.u0, f.v0 };
        uv[3] = Vec2{ f.u0, f.v1 };
    }
}

bool Parser::LoadNoteData(const wstring& songName, Ptr<CNoteReceptor>* nrArr)
{
    // wstring → string
    wstring tempPath = PathManager::GetInstance()->GetContentsPath();
    tempPath += L"Resources\\SongData\\" + songName + L".json";

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

    string filePath = path;

    FILE* fp = nullptr;
    fopen_s(&fp, filePath.c_str(), "rb");
    if (!fp)
        return false;

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document doc;
    doc.ParseStream(is);
    fclose(fp);

    if (!doc.IsObject() || !doc.HasMember("notes"))
        return false;

    const auto& notes = doc["notes"];

    struct DiffPair
    {
        const char* name;
    };

    DiffPair diffs[] =
    {
        { "easy" },
        { "normal" },
        { "hard" }
    };

    if (!notes.HasMember(diffs[int(DIFF)].name))
        return false;

    const auto& noteArr = notes[diffs[int(DIFF)].name];
    if (!noteArr.IsArray())
        return false;

    for (auto& n : noteArr.GetArray())
    {
        float t = n["t"].GetFloat();
        int d = n["d"].GetInt();
        nrArr[d]->tapTimings.push_back(t);
        // l이 있을 경우엔 pressTiming에 넣어주기
        if (n.HasMember("l"))
            nrArr[d]->pressTimings.push_back({ t, n["l"].GetFloat()});
    }

    for (auto& n : noteArr.GetArray())
    {
        // timings 작업이 끝나면 note의 배열 사이즈를 동일하게 resize해준다 (메모리 절약)
        int d = n["d"].GetInt();

        nrArr[d]->m_tapNoteVec.resize(nrArr[d]->tapTimings.size());
        nrArr[d]->m_pressNoteVec.resize(nrArr[d]->pressTimings.size());
    }


    return true;
}

bool Parser::LoadEventData(const wstring& songName, Ptr<CEventManager> eventManager)
{
    // wstring → string (path 구성 동일)
    wstring tempPath = PathManager::GetInstance()->GetContentsPath();
    tempPath += L"Resources\\SongData\\" + songName + L".json";

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

    FILE* fp = nullptr;
    fopen_s(&fp, path.c_str(), "rb");
    if (!fp)
        return false;

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document doc;
    doc.ParseStream(is);
    fclose(fp);

    // events 체크
    if (!doc.IsObject() || !doc.HasMember("events"))
        return false;

    const auto& events = doc["events"];
    if (!events.IsArray())
        return false;

    // 이벤트 파싱
    for (auto& ev : events.GetArray())
    {
        if (!ev.HasMember("t") || !ev.HasMember("e"))
            continue;

        float timing = ev["t"].GetFloat();
        const char* type = ev["e"].GetString();

        // FocusCamera
        if (strcmp(type, "FocusCamera") == 0)
        {
            if (!ev.HasMember("v"))
                continue;

            int value = 0;
            const auto& v = ev["v"];

            if (v.IsInt())
            {
                // "v": 1
                value = v.GetInt();
            }
            else if (v.IsObject() && v.HasMember("char") && v["char"].IsInt())
            {
                // "v": { "char": 1 }
                value = v["char"].GetInt();
            }
            else
            {
                continue; // 예상하지 않은 형식
            }

            eventManager->AddCameraEvent(CameraEventInfo(timing, value));
        }
        // PlayAnimation (HEY 고정)
        else if (strcmp(type, "PlayAnimation") == 0)
        {
            eventManager->AddAnimEvent(timing);
        }
    }

    return true;
}

void AtlasFrame::SaveAtlasFrame(FILE* file) const
{
    size_t len = name.size();
    fwrite(&len, sizeof(size_t), 1, file);
    fwrite(name.data(), sizeof(char), len, file);

    fwrite(&x, sizeof(int), 1, file);
    fwrite(&y, sizeof(int), 1, file);
    fwrite(&width, sizeof(int), 1, file);
    fwrite(&height, sizeof(int), 1, file);

    fwrite(&frameX, sizeof(int), 1, file);
    fwrite(&frameY, sizeof(int), 1, file);
    fwrite(&frameWidth, sizeof(int), 1, file);
    fwrite(&frameHeight, sizeof(int), 1, file);

    fwrite(&rotated, sizeof(bool), 1, file);

    fwrite(&u0, sizeof(float), 1, file);
    fwrite(&v0, sizeof(float), 1, file);
    fwrite(&u1, sizeof(float), 1, file);
    fwrite(&v1, sizeof(float), 1, file);
}

void AtlasFrame::LoadAtlasFrame(FILE* file)
{
    size_t len;
    fread(&len, sizeof(size_t), 1, file);

    name.resize(len);
    fread(name.data(), sizeof(char), len, file);

    fread(&x, sizeof(int), 1, file);
    fread(&y, sizeof(int), 1, file);
    fread(&width, sizeof(int), 1, file);
    fread(&height, sizeof(int), 1, file);

    fread(&frameX, sizeof(int), 1, file);
    fread(&frameY, sizeof(int), 1, file);
    fread(&frameWidth, sizeof(int), 1, file);
    fread(&frameHeight, sizeof(int), 1, file);

    fread(&rotated, sizeof(bool), 1, file);

    fread(&u0, sizeof(float), 1, file);
    fread(&v0, sizeof(float), 1, file);
    fread(&u1, sizeof(float), 1, file);
    fread(&v1, sizeof(float), 1, file);
}

void AtlasAnimation::SaveAtlasAnimation(FILE* file) const
{
    size_t frameCount = frames.size();
    fwrite(&frameCount, sizeof(size_t), 1, file);

    for (const auto& frame : frames)
    {
        frame.SaveAtlasFrame(file);
    }
}

void AtlasAnimation::LoadAtlasAnimation(FILE* file)
{
    size_t frameCount;
    fread(&frameCount, sizeof(size_t), 1, file);

    frames.resize(frameCount);

    for (size_t i = 0; i < frameCount; ++i)
    {
        frames[i].LoadAtlasFrame(file);
    }
}
