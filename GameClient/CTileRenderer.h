#pragma once
#include "CRenderComponent.h"
#include "ATileMap.h"
#include "StructuredBuffer.h"

struct SpriteInfo
{
	Vec2 leftTop;
	Vec2 slice;
};

class CTileRenderer : public CRenderComponent
{
public:
	CTileRenderer();
	virtual ~CTileRenderer();

public:
	virtual void FinalTick() override;
	virtual void Render() override;
	virtual void CreateMaterial() override;

public:
	void SetTileMap(Ptr<ATileMap> _tileMap);

private:
	Ptr<ATileMap> m_tileMap;
	vector<SpriteInfo> m_vecSpriteInfo;
	Ptr<StructuredBuffer> m_buffer;
};

