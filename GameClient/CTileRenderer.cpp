#include "pch.h"
#include "CTileRenderer.h"
#include "AssetManager.h"
#include "CTransform.h"

CTileRenderer::CTileRenderer()
	:CRenderComponent(COMPONENT_TYPE::TILE_RENDERER)
{
	m_buffer = new StructuredBuffer;
}

CTileRenderer::CTileRenderer(const CTileRenderer& _Origin)
	: CRenderComponent(_Origin)
	, m_tileMap(_Origin.m_tileMap)
	, m_vecSpriteInfo(_Origin.m_vecSpriteInfo)
	, m_buffer(nullptr)
{
	m_buffer = new StructuredBuffer;
}

CTileRenderer::~CTileRenderer()
{
}

void CTileRenderer::Init()
{
	CRenderComponent::Init();

	SetTileMap(m_tileMap);
}

void CTileRenderer::FinalTick()
{
	if (m_tileMap == nullptr)
	{

	}
}

void CTileRenderer::Render()
{
	m_buffer->Binding(20);

	GetMaterial()->SetTexture(TEX_0, m_tileMap->GetAtlas());

	GetMaterial()->SetScalar(INT_0, m_tileMap->GetRow());
	GetMaterial()->SetScalar(INT_1, m_tileMap->GetCol());
	GetMaterial()->SetScalar(VEC2_0, m_vecSpriteInfo[0].leftTop);
	GetMaterial()->SetScalar(VEC2_1, m_vecSpriteInfo[0].slice);

	GetMaterial()->Binding();

	GetMesh()->Render();

	m_buffer->Clear();
}

void CTileRenderer::CreateMaterial()
{
	wstring MeshName = L"RectMesh";
	wstring MtrlName = L"TileMtrl";
	wstring ShaderName = L"TileShader";
	wstring FilePath = L"Shaders\\tile.hlsl";
	string VS = "VS_Tile";
	string PS = "PS_Tile";

	// RectMesh 설정
	SetMesh(AssetManager::GetInstance()->Find<AMesh>(MeshName));

	// 재질 생성
	Ptr<AMaterial> pMtrl = AssetManager::GetInstance()->Find<AMaterial>(MtrlName);

	// 찾는 재질이 없으면 생성한다.
	if (nullptr == pMtrl)
	{
		pMtrl = new AMaterial;
		pMtrl->SetName(MtrlName);

		// 쉐이더를 찾아서 재질에 세팅해준다.
		Ptr<AGraphicShader> pShader = AssetManager::GetInstance()->Find<AGraphicShader>(ShaderName);

		// 찾은 or 생성한 쉐이더를 재질에 설정해주고, 재질도 에셋매니저에 등록한다.
		pMtrl->SetShader(pShader);
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
		AssetManager::GetInstance()->AddAsset(pMtrl->GetName(), pMtrl.Get());
	}

	SetMaterial(pMtrl);
}

void CTileRenderer::SetTileMap(Ptr<ATileMap> _tileMap)
{
	m_tileMap = _tileMap;

	if (nullptr == m_tileMap)
		return;

	// 이전 정보 리셋
	m_vecSpriteInfo.clear();

	// 크키 조정
	UINT Row = m_tileMap->GetRow();
	UINT Col = m_tileMap->GetCol();
	Vec2 TileSize = m_tileMap->GetTileSize();
	Vec3 vScale = Vec3(TileSize.x * (float)Row, TileSize.y * (float)Col, 1.f);
	Transform()->SetRelativeScale(vScale);

	// TileMap 의 Sprite 의 UV 정보를 받아옴
	vector<Ptr<ASprite>> vecSprites = m_tileMap->GetSprites();

	for (int i = 0; i < vecSprites.size(); ++i)
	{
		SpriteInfo info = {};

		if (nullptr != vecSprites[i])
		{
			info.leftTop = vecSprites[i]->GetLeftTopUV();
			info.slice = vecSprites[i]->GetSliceUV();
		}

		m_vecSpriteInfo.push_back(info);
	}

	if (m_buffer->GetBufferSize() < sizeof(SpriteInfo) * m_vecSpriteInfo.size())
		m_buffer->Create(sizeof(SpriteInfo), m_vecSpriteInfo.size(), SB_TYPE::SRV_ONLY, true);

	m_buffer->SetData(m_vecSpriteInfo.data(), sizeof(SpriteInfo) * m_vecSpriteInfo.size());
}

void CTileRenderer::SaveToSceneFile(FILE* _File)
{
	CRenderComponent::SaveToSceneFile(_File);

	SaveAssetRef(_File, m_tileMap.Get());
}

void CTileRenderer::LoadFromSceneFile(FILE* _File)
{
	CRenderComponent::LoadFromSceneFile(_File);

	m_tileMap = LoadAssetRef<ATileMap>(_File);

	SetTileMap(m_tileMap);
}
