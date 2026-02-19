#pragma once
#include "Asset.h"
#include "ATexture.h"

class ASprite : public Asset
{
public:
	ASprite();
	virtual ~ASprite();

public:
	GET_SET(Ptr<ATexture>, Atlas);
	GET_SET(Vec2, LeftTopUV);
	GET_SET(Vec2, SliceUV);
	
private:
	Ptr<ATexture> m_Atlas;

	Vec2 m_LeftTopUV;
	Vec2 m_SliceUV;
};

