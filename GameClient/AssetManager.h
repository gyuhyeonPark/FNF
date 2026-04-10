#pragma once
#include "Asset.h"
#include "ATexture.h"
#include "AMaterial.h"
#include "ASprite.h"
#include "AScene.h"

class AssetManager : public SingletonClass<AssetManager>
{
	SINGLE(AssetManager);
public:
	~AssetManager();

private:
	void CreateEngineMesh();
	void CreateEngineShader();
	void CreateEngineTexture();
	void CreateEngineMaterial();
	void CreateEngineSprite();

public:
	void Init();
	bool IsChanged()
	{
		bool Changed = m_Changed;
		m_Changed = false;
		return Changed;
	}

public:
	Ptr<Asset> FindAsset(ASSET_TYPE _type, const wstring& _key);
	void AddAsset(const wstring& _key, Ptr<Asset> asset);

	void GetAssetNames(ASSET_TYPE _type, vector<wstring>& _vec);

	template <typename T>
	Ptr<T> Find(const wstring& _key);

	template<typename T>
	Ptr<T> Load(const wstring& _Key, const wstring& _RelativePath);

private:
	map<wstring, Ptr<Asset>> m_assetMap[(UINT)ASSET_TYPE::END];
	bool m_Changed;
};

template<typename T>
ASSET_TYPE GetAssetType()
{
	if constexpr (std::is_same_v<T, AMesh>)
		return ASSET_TYPE::MESH;
	else if constexpr (std::is_same_v<T, AMaterial>)
		return ASSET_TYPE::MATERIAL;
	else if constexpr (std::is_same_v<T, AGraphicShader>)
		return ASSET_TYPE::GRAPHICSHADER;
	else if constexpr (std::is_same_v<T, ATexture>)
		return ASSET_TYPE::TEXTURE;
	else if constexpr (std::is_same_v<T, ASprite>)
		return ASSET_TYPE::SPRITE;
	else if constexpr (std::is_same_v<T, AFlipbook>)
		return ASSET_TYPE::FLIPBOOK;
	else if constexpr (std::is_same_v<T, ATileMap>)
		return ASSET_TYPE::TILEMAP;	
	else if constexpr (std::is_same_v<T, AScene>)
		return ASSET_TYPE::SCENE;

	return ASSET_TYPE::END;
}

template<typename T>
Ptr<T> AssetManager::Find(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<Asset>>::iterator iter = m_assetMap[(UINT)Type].find(_Key);

	if (iter == m_assetMap[(UINT)Type].end())
		return nullptr;

	return (T*)iter->second.Get();
}

template<typename T>
inline Ptr<T> AssetManager::Load(const wstring& _Key, const wstring& _RelativePath)
{
	// 동일 키로 먼저 등록된 에셋이 있는 지 확인.
	Ptr<T> pAsset = Find<T>(_Key);

	if (pAsset != nullptr)
		return pAsset;

	pAsset = new T;

	pAsset->Load(PathManager::GetInstance()->GetContentsPath() + _RelativePath);

	ASSET_TYPE type = GetAssetType<T>();

	// 에셋 등록
	m_assetMap[(UINT)type].insert({ _Key, pAsset.Get() });

	// 에셋이 매니저에 등록될 때 사용된 key와,
	// 자신이 어떤 경로에 있는 파일로부터 로딩된 에셋인지 스스로 알 수 있도록 하기.
	pAsset->SetKey(_Key);
	pAsset->SetRelativePath(_RelativePath);

	m_Changed = true;

	return pAsset;
}

template<typename T>
Ptr<T> LoadAssetRef(FILE* _File)
{
	// Asset 이 Null 인지 아닌지 저장
	bool IsNull = false;
	fread(&IsNull, sizeof(bool), 1, _File);

	// Asset 의 Key, RelativePath 저장
	if (IsNull)
	{
		wstring Key = LoadWString(_File);
		wstring RelativePath = LoadWString(_File);
		return AssetManager::GetInstance()->Load<T>(Key, RelativePath);
	}

	return nullptr;
}


#define FIND(Type, Key) AssetManager::GetInstance()->Find<Type>(Key)
#define LOAD(Type, AssetPath) AssetManager::GetInstance()->Load<Type>(AssetPath, AssetPath)
