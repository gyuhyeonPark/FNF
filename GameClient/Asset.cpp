#include "pch.h"
#include "Asset.h"

Asset::Asset(ASSET_TYPE type)
	: m_key(), m_relativePath(), m_type(type)
{
}

Asset::~Asset()
{
}
