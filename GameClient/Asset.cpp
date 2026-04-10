#include "pch.h"
#include "Asset.h"

Asset::Asset(ASSET_TYPE type)
	: m_key(), m_relativePath(), m_type(type)
{
}

Asset::Asset(const Asset& _Orgin)
	: Entity(_Orgin)
	, m_key(_Orgin.m_key)
	, m_relativePath()
	, m_type(_Orgin.m_type)
{
}

Asset::~Asset()
{
}
