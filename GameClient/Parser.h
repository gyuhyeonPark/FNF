#pragma once

struct AtlasFrame
{
    std::string name;

    // Atlas pixel rect
    int x, y;
    int width, height;

    // Original frame info
    int frameX, frameY;
    int frameWidth, frameHeight;

    bool rotated;

    // UV (계산 후)
    float u0, v0;
    float u1, v1;

public:
    void SaveAtlasFrame(FILE* file) const;
    void LoadAtlasFrame(FILE* file);
};

struct AtlasAnimation
{
    std::vector<AtlasFrame> frames;

public:
    void SaveAtlasAnimation(FILE* file) const;
    void LoadAtlasAnimation(FILE* file);
};

class CNoteReceptor;
class CEventManager;

class Parser : public SingletonClass<Parser>
{
    SINGLE(Parser);

public:
    bool LoadTextureAtlas(const std::string& xmlName, int atlasWidth, int atlasHeight,
        std::unordered_map<std::string, AtlasAnimation>& outAnimations);
    void BuildUV(const AtlasFrame& f, Vec2 uv[4]);

public:
    bool LoadNoteData(const wstring& songName, Ptr<CNoteReceptor>* nrArr);
    bool LoadEventData(const wstring& songName, Ptr<CEventManager> eventManager);

private:
    string m_path;
};

