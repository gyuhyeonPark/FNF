#pragma once
#include "Entity.h"
class Asset : public Entity
{
private:
	wstring m_key;			// 에셋을 찾을 때의 키값.
	wstring m_relativePath; // 상대 경로

	const ASSET_TYPE m_type;

public:
	ASSET_TYPE GetType() { return m_type; }
	const wstring GetKey() { return m_key; }
	const wstring& GetRelativePath() { return m_relativePath; }

private:
	void SetKey(const wstring& _key) { m_key = _key; }
	void SetRelativePath(const wstring& _path) { m_relativePath = _path; }

public:
	virtual int Load(const wstring& _FilePath) { return S_OK; }

public:
	// 기본 생성자 제거, 자신이 어떤 타입의 Asset인지 먼저 입력으로 받는다.
	Asset(ASSET_TYPE type);
	virtual ~Asset();

	friend class AssetManager;
};


