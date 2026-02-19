#include "pch.h"
#include "AssetManager.h"
#include "PathManager.h"
#include "AssetKeys.h"

void AssetManager::Init()
{
	CreateEngineMesh();
	CreateEngineShader();
	CreateEngineTexture();
	CreateEngineMaterial();
	CreateEngineSprite();
}

void AssetManager::CreateEngineMesh()
{
	Ptr<AMesh> mesh;

	// ========
	// RectMesh
	// ========
	VertexInfo arrVtx[4] = {};

	arrVtx[0].position = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].uv = Vec2(0.f, 0.f);
	arrVtx[0].color = Vec4(1.f, 0.f, 0.f, 0.f);

	arrVtx[1].position = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].uv = Vec2(1.f, 0.f);
	arrVtx[1].color = Vec4(0.f, 0.f, 1.f, 0.f);

	arrVtx[2].position = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].uv = Vec2(1.f, 1.f);
	arrVtx[2].color = Vec4(0.f, 1.f, 0.f, 0.f);

	arrVtx[3].position = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].uv = Vec2(0.f, 1.f);
	arrVtx[3].color = Vec4(1.f, 0.f, 0.f, 0.f);

	UINT arrIdx[6] = { 0, 2, 3, 0, 1, 2 };

	// 사각형 메쉬 생성
	mesh = new AMesh;
	mesh->Create(arrVtx, 4, arrIdx, 6);
	AddAsset(L"RectMesh", mesh.Get());

	// ==================
	// RectMesh_LineStrip
	// ==================
	arrIdx[0] = 0; 	arrIdx[1] = 1;	arrIdx[2] = 2;	arrIdx[3] = 3; arrIdx[4] = 0;
	mesh = new AMesh;
	mesh->Create(arrVtx, 4, arrIdx, 5);
	AddAsset(L"RectMesh_LineStrip", mesh.Get());

	// ===============
	// 원 (CircleMesh)
	// ===============
	vector<VertexInfo>	vecVtx;
	vector<UINT> vecIdx;

	// 중점
	VertexInfo v;
	v.position = Vec3(0.f, 0.f, 0.f);
	v.uv = Vec2(0.5f, 0.5f);
	v.color = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	float Theta = 0.f;
	float Radius = 0.5f;
	float Slice = 50.f;

	// 원의 테두리 정점 추가
	for (int i = 0; i < (int)Slice + 1; ++i)
	{
		v.position = Vec3(Radius * cosf(Theta), Radius * sinf(Theta), 0.f);
		//v.vUV = Vec2(0.5f, 0.5f);
		v.color = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		Theta += XM_2PI / Slice;
	}

	// 인덱스
	for (int i = 0; i < (int)Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	mesh = new AMesh;
	mesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"CircleMesh", mesh.Get());

	// ====================
	// CircleMesh_LineStrip
	// ====================
	vecIdx.clear();
	for (int i = 0; i < (int)Slice + 1; ++i)
	{
		vecIdx.push_back(i + 1);
	}


	mesh = new AMesh;
	mesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"CircleMesh_LineStrip", mesh.Get());
}

void AssetManager::CreateEngineShader()
{
	Ptr<AGraphicShader> shader;
	shader = new AGraphicShader();
	shader->CreateVertexShader(L"Shaders\\AtlasUVShader.hlsl", "VSMainAnim");
	shader->CreatePixelShader(L"Shaders\\AtlasUVShader.hlsl", "PSMain");
	AddAsset(L"AtlasUVShader", shader.Get());

	shader = new AGraphicShader();
	shader->CreateVertexShader(L"Shaders\\EffectShader.hlsl", "VSMainAnim");
	shader->CreatePixelShader(L"Shaders\\EffectShader.hlsl", "PSMain");
	AddAsset(L"VolatileShader", shader.Get());

	shader = new AGraphicShader;
	shader->CreateVertexShader(L"Shaders\\RSG.hlsl", "VS_Std2D");
	shader->CreatePixelShader(L"Shaders\\RSG.hlsl", "PS_Std2D");
	shader->SetRSType(RS_TYPE::CULL_NONE);
	shader->SetBSType(BS_TYPE::ALPHABLEND);
	AddAsset(L"RSGShader", shader.Get());

	// ===========
	// Std2DShader
	// ===========
	shader = new AGraphicShader;
	shader->CreateVertexShader(L"Shaders\\std2d.hlsl", "VS_Std2D");
	shader->CreatePixelShader(L"Shaders\\std2d.hlsl", "PS_Std2D");
	shader->SetRSType(RS_TYPE::CULL_NONE);
	AddAsset(L"Std2DShader", shader.Get());

	// ===============
	// DbgRenderShader
	// ===============
	shader = new AGraphicShader;
	shader->CreateVertexShader(L"Shaders\\dbg.hlsl", "VS_Debug");
	shader->CreatePixelShader(L"Shaders\\dbg.hlsl", "PS_Debug");
	shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	shader->SetRSType(RS_TYPE::CULL_NONE);
	shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	shader->SetBSType(BS_TYPE::DEFAULT);
	AddAsset(L"DbgShader", shader.Get());
}

void AssetManager::CreateEngineTexture()
{
	wstring FilePath = PathManager::GetInstance()->GetContentsPath();

	Ptr<ATexture> pTex = nullptr;

	Load<ATexture>(L"bf", L"Resources\\bf\\BOYFRIEND.png");

	Load<ATexture>(L"daddy", L"Resources\\daddy\\daddyDearest.png");

	Load<ATexture>(L"stageback", L"Resources\\bg\\stageback.png");
	Load<ATexture>(L"stagefront", L"Resources\\bg\\stagefront.png");
	Load<ATexture>(L"notereceptor", L"Resources\\ui\\noteStrumline.png");
	Load<ATexture>(L"ready", L"Resources\\ui\\ready.png");
	Load<ATexture>(L"set", L"Resources\\ui\\set.png");
	Load<ATexture>(L"go", L"Resources\\ui\\go.png");
	Load<ATexture>(L"notes", L"Resources\\ui\\notes.png");

	Load<ATexture>(L"effectbluehold", L"Resources\\ui\\holdCoverBlue.png");
	Load<ATexture>(L"effectgreenhold", L"Resources\\ui\\holdCoverGreen.png");
	Load<ATexture>(L"effectpurplehold", L"Resources\\ui\\holdCoverPurple.png");
	Load<ATexture>(L"effectredhold", L"Resources\\ui\\holdCoverRed.png");

	Load<ATexture>(L"NoteHoldBar", L"Resources\\ui\\NOTE_hold_assets.png");

	// ========================================================
	// 아래는 기존에 ContentsPath + "Texture\\..." 쓰던 부분

	Load<ATexture>(L"Fighter", L"Texture\\Fighter.bmp");
	Load<ATexture>(L"Missile", L"Texture\\missile.png");
	Load<ATexture>(L"Link", L"Texture\\link.png");
	Load<ATexture>(L"TileAtlas", L"Texture\\TILE.bmp");

}

void AssetManager::CreateEngineMaterial()
{
	// 재질 만들기
	Ptr<AMaterial> pMtrl = nullptr;

	// Atlas Character Material 제작
	pMtrl = new AMaterial;
	pMtrl->SetName(L"Player");
	pMtrl->SetShader(Find<AGraphicShader>(L"AtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"bf"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"NoteReceptor");
	pMtrl->SetShader(Find<AGraphicShader>(L"AtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"notereceptor"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Notes");
	pMtrl->SetShader(Find<AGraphicShader>(L"AtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"notes"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Daddy");
	pMtrl->SetShader(Find<AGraphicShader>(L"AtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"daddy"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"RSG");
	pMtrl->SetShader(Find<AGraphicShader>(L"RSGShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"ready"));
	pMtrl->SetTexture(TEX_1, Find<ATexture>(L"set"));
	pMtrl->SetTexture(TEX_2, Find<ATexture>(L"go"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());


	// Effects
	pMtrl = new AMaterial;
	pMtrl->SetName(L"EffectBlueHold");
	pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectbluehold"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"EffectBlueVolatile");
	pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectbluehold"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"EffectGreenHold");
	pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectgreenhold"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"EffectGreenVolatile");
	pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectgreenhold"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"EffectPurpleHold");
	pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectpurplehold"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"EffectPurpleVolatile");
	pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectpurplehold"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"EffectRedHold");
	pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectredhold"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"EffectRedVolatile");
	pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectredhold"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	// =========
	// Std2DMtrl 
	// =========
	pMtrl = new AMaterial;
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(Find<AGraphicShader>(L"Std2DShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"Fighter"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	// =======
	// DbgMtrl 
	// =======
	pMtrl = new AMaterial;
	pMtrl->SetName(L"DbgMtrl");
	pMtrl->SetShader(Find<AGraphicShader>(L"DbgShader"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());
}

void AssetManager::CreateEngineSprite()
{
	Ptr<ATexture> pAtlas = FIND(ATexture, L"Link");
	float Width = pAtlas->GetWidth();
	float Height = pAtlas->GetHeight();
	Vec2 SlicePixel = Vec2(120.f, 130.f);

	Ptr<ASprite> pSprite = nullptr;
	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMD_%d", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 4.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkML_%d", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 5.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMU_%d", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 6.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMR_%d", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 7.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	pAtlas = FIND(ATexture, L"NoteHoldBar");
	SlicePixel = Vec2(52, 85);
	Width = pAtlas->GetWidth();
	Height = pAtlas->GetHeight();

	for (int i = 0; i < 8; i++)
	{
		pSprite = new ASprite;
		pSprite->SetName(noteHoldNames[i]);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, 0.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	Ptr<AFlipbook> pFlipbook = nullptr;

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Link_MoveDown");

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMD_%d", i);
		pFlipbook->AddSprite(FIND(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Link_MoveLeft");

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkML_%d", i);
		pFlipbook->AddSprite(FIND(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Link_MoveUp");

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMU_%d", i);
		pFlipbook->AddSprite(FIND(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Link_MoveRight");

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMR_%d", i);
		pFlipbook->AddSprite(FIND(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());


	// ===========
	// Tile Sprite
	// ===========
	pAtlas = FIND(ATexture, L"TileAtlas");

	Width = pAtlas->GetWidth();
	Height = pAtlas->GetHeight();
	SlicePixel = Vec2(64.f, 64.f);

	int Count = 0;
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 8; ++j, ++Count)
		{
			wchar_t Buff[50] = {};
			swprintf_s(Buff, L"TileSprite_%d", Count);

			pSprite = new ASprite;
			pSprite->SetName(Buff);
			pSprite->SetAtlas(pAtlas);
			pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)j, (SlicePixel.y / Height) * i));
			pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
			AddAsset(pSprite->GetName(), pSprite.Get());
		}
	}

	// =======
	// TileMap
	// =======
	Ptr<ATileMap> pTileMap = nullptr;

	int row = 30;
	int col = 30;

	pTileMap = new ATileMap;
	pTileMap->SetName(L"TestTileMap");
	pTileMap->SetRowCol(row, col);
	pTileMap->SetTileSize(Vec2(64.f, 64.f));
	pTileMap->SetAtlas(FIND(ATexture, L"TileAtlas"));

	for (int i = 0; i < row; ++i)
		for (int j = 0; j < col; ++j)
			pTileMap->SetSprite(i, j, FIND(ASprite, L"TileSprite_1"));


	AddAsset(pTileMap->GetName(), pTileMap.Get());
}