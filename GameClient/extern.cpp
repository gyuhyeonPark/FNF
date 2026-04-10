#include "extern.h"

ConstantStruct	g_Trans = {};
GlobalData		g_Global = {};

const char* ASSET_TYPE_STR[(UINT)ASSET_TYPE::END] =
{
	"MESH",
	"TEXTURE",
	"SOUND",
	"GRAPHICSHADER",
	"COMPUTESHADER",
	"MATERIAL",
	"SCENE",
	"SPRITE",
	"FLIPBOOK",
	"TILEMAP",
	"PREFAB",
};

const char* ToString(ASSET_TYPE _Type)
{
	return ASSET_TYPE_STR[(UINT)_Type];
}

std::string WStringToUTF8(const std::wstring& ws)
{
	if (ws.empty()) return {};
	int size = WideCharToMultiByte(CP_UTF8, 0, ws.data(), (int)ws.size(), nullptr, 0, nullptr, nullptr);
	std::string s(size, '\0');
	WideCharToMultiByte(CP_UTF8, 0, ws.data(), (int)ws.size(), s.data(), size, nullptr, nullptr);
	return s;
}