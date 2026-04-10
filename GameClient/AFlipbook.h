#pragma once
#include "Asset.h"
#include "ASprite.h"

class AFlipbook : public Asset
{
public:
	AFlipbook();
	virtual ~AFlipbook();

public:
	void AddSprite(Ptr<ASprite> sprite) { m_vecSprite.push_back(sprite); }
	void SetSprite(int idx, Ptr<ASprite> sprite)
	{
		if (m_vecSprite.size() <= idx)
			m_vecSprite.resize(idx + 1);
	
		m_vecSprite[idx] = sprite;
	}

	Ptr<ASprite> GetSprite(int idx) { return m_vecSprite[idx]; }

	UINT GetSpriteCount() { return m_vecSprite.size(); }

	virtual int Load(const wstring& _FilePath) override;
	virtual int Save(const wstring& _FilePath) override;

private:
	vector<Ptr<ASprite>> m_vecSprite;
};

