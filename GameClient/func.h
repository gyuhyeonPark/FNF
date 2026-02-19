#pragma once

// Task
void CreateObject(class GameObject* object, int layeridx);

// DebugRender
void DrawDebugRect(Vec3 pos, Vec3 scale, Vec3 rot, Vec4 color, float duration, bool depthTest = false);
void DrawDebugRect(Matrix matWorld, Vec4 color, float duration, bool depthTest = false);

void DrawDebugCircle(Vec3 pos, float radius, Vec4 color, float duration, bool depthTest = false);
