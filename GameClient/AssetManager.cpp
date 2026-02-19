#include "pch.h"
#include "AssetManager.h"

AssetManager::AssetManager()
{

}
AssetManager::~AssetManager()
{

}

Ptr<Asset> AssetManager::FindAsset(ASSET_TYPE _type, const wstring& _key)
{
	map<wstring, Ptr<Asset>>::iterator iter = m_assetMap[(UINT)_type].find(_key);

	if (iter == m_assetMap[(UINT)_type].end())
		return nullptr;

	return iter->second;
}

void AssetManager::AddAsset(const wstring& _key, Ptr<Asset> asset)
{
	if (m_assetMap[UINT(asset->GetType())].find(_key) != m_assetMap[UINT(asset->GetType())].end())
		return;

	asset->SetKey(_key);

	m_assetMap[UINT(asset->GetType())].insert({ _key , asset });
}

void AssetManager::GetAssetNames(ASSET_TYPE _type, vector<wstring>& _vec)
{
	for (const auto& pair : m_assetMap[(UINT)_type])
	{
		_vec.push_back(pair.first);
	}
}