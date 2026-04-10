#pragma once
#include "pch.h"

extern const char* ASSET_TYPE_STR[(UINT)ASSET_TYPE::END];
const char* ToString(ASSET_TYPE _Type);
std::string WStringToUTF8(const std::wstring& ws);