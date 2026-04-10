#pragma once
#include "Asset.h"
#include "IResourceManager.h"

// Task
void CreateObject(class GameObject* object, int layeridx);
void ChangeScene(const wstring& _NextSceneName);
void ChangeSceneState(SCENE_STATE _NextState);

// DebugRender
void DrawDebugRect(Vec3 pos, Vec3 scale, Vec3 rot, Vec4 color, float duration, bool depthTest = false);
void DrawDebugRect(Matrix matWorld, Vec4 color, float duration, bool depthTest = false);

void DrawDebugCircle(Vec3 pos, float radius, Vec4 color, float duration, bool depthTest = false);

void SaveWString(FILE* _File, const wstring& _String);
wstring LoadWString(FILE* _File);

void SaveAssetRef(FILE* _File, Asset* _Asset);


void CreateSongEditorScene();

void CreateMainMenuScene();
void CreateScene(STAGENUM stage, DIFFICULTIES diff, wstring sceneName);

void CreateLoadingScene(STAGENUM nextStage, wstring nextSceneName);

// 0 ~ 1 로 제한
float Saturate(float _Data);