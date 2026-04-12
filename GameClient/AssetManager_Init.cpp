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
	shader->CreateVertexShader(L"Shaders\\UIAtlasUVShader.hlsl", "VSMainAnim");
	shader->CreatePixelShader(L"Shaders\\UIAtlasUVShader.hlsl", "PSMain");
	shader->SetRSType(RS_TYPE::CULL_NONE);
	shader->SetBSType(BS_TYPE::ALPHABLEND);
	AddAsset(L"UIAtlasUVShader", shader.Get());

	shader = new AGraphicShader();
	shader->CreateVertexShader(L"Shaders\\NoteShader.hlsl", "VS_Sprite");
	shader->CreatePixelShader(L"Shaders\\NoteShader.hlsl", "PS_Sprite");
	AddAsset(L"NoteShader", shader.Get());

	shader = new AGraphicShader();
	shader->CreateVertexShader(L"Shaders\\NoteBarShader.hlsl", "VS_Sprite");
	shader->CreatePixelShader(L"Shaders\\NoteBarShader.hlsl", "PS_Sprite");
	AddAsset(L"NoteBarShader", shader.Get());

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

	shader = new AGraphicShader;
	shader->CreateVertexShader(L"Shaders\\Combo.hlsl", "VS_Std2D");
	shader->CreatePixelShader(L"Shaders\\Combo.hlsl", "PS_Std2D");
	shader->SetRSType(RS_TYPE::CULL_NONE);
	shader->SetBSType(BS_TYPE::ALPHABLEND);
	AddAsset(L"ComboShader", shader.Get());

	shader = new AGraphicShader;
	shader->CreateVertexShader(L"Shaders\\RSG.hlsl", "VS_Std2D");
	shader->CreatePixelShader(L"Shaders\\RSG.hlsl", "PS_Std2D");
	shader->SetRSType(RS_TYPE::CULL_NONE);
	shader->SetBSType(BS_TYPE::ALPHABLEND);
	shader->SetDSType(DS_TYPE::TRANS_PARENT);
	AddAsset(L"TitleBlinkShader", shader.Get());

	// ===========
	// Std2DShader
	// ===========
	shader = new AGraphicShader;
	shader->CreateVertexShader(L"Shaders\\std2d.hlsl", "VS_Std2D");
	shader->CreatePixelShader(L"Shaders\\std2d.hlsl", "PS_Std2D");
	shader->SetRSType(RS_TYPE::CULL_NONE);
	AddAsset(L"Std2DShader", shader.Get());

	shader = new AGraphicShader;
	shader->CreateVertexShader(L"Shaders\\std2dLight.hlsl", "VS_Std2D");
	shader->CreatePixelShader(L"Shaders\\std2dLight.hlsl", "PS_Std2D");
	shader->SetRSType(RS_TYPE::CULL_NONE);
	AddAsset(L"Std2DLightShader", shader.Get());

	// ===============
	// BillboardShader
	// ===============
	shader = new AGraphicShader;
	shader->SetName(L"BillboardShader");
	shader->CreateVertexShader(L"shaders\\billboard.hlsl", "VS_Billboard");
	shader->CreatePixelShader(L"shaders\\billboard.hlsl", "PS_Billboard");
	shader->SetBSType(BS_TYPE::DEFAULT);
	shader->SetRSType(RS_TYPE::CULL_NONE);
	AddAsset(shader->GetName(), shader.Get());

	// ============
	// SpriteShader
	// ============
	shader = new AGraphicShader;
	shader->SetName(L"SpriteShader");
	shader->CreateVertexShader(L"shaders\\sprite.hlsl", "VS_Sprite");
	shader->CreatePixelShader(L"shaders\\sprite.hlsl", "PS_Sprite");
	shader->SetBSType(BS_TYPE::DEFAULT);
	shader->SetRSType(RS_TYPE::CULL_NONE);
	AddAsset(shader->GetName(), shader.Get());

	// ==============
	// FlipbookShader
	// ==============
	shader = new AGraphicShader;
	shader->SetName(L"FlipbookShader");
	shader->CreateVertexShader(L"shaders\\flipbook.hlsl", "VS_Flipbook");
	shader->CreatePixelShader(L"shaders\\flipbook.hlsl", "PS_Flipbook");
	shader->SetBSType(BS_TYPE::DEFAULT);
	shader->SetRSType(RS_TYPE::CULL_NONE);
	AddAsset(shader->GetName(), shader.Get());

	// =============
	// TileMashader
	// =============
	// 찾는 쉐이더가 없으면 만들어서 에셋매니저에 등록해둔다

	shader = new AGraphicShader;
	shader->SetName(L"TileShader");
	shader->CreateVertexShader(L"shaders\\tile.hlsl", "VS_Tile");
	shader->CreatePixelShader(L"shaders\\tile.hlsl", "PS_Tile");
	shader->SetBSType(BS_TYPE::DEFAULT);
	shader->SetRSType(RS_TYPE::CULL_NONE);
	AddAsset(shader->GetName(), shader.Get());


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
	Load<ATexture>(L"gf", L"Resources\\gf\\GF_assets.png");

	Load<ATexture>(L"daddy", L"Resources\\daddy\\daddyDearest.png");

	Load<ATexture>(L"stageback", L"Resources\\bg\\stageback.png");
	Load<ATexture>(L"stagefront", L"Resources\\bg\\stagefront.png");
	Load<ATexture>(L"notereceptor", L"Resources\\ui\\noteStrumline.png");
	Load<ATexture>(L"ready", L"Resources\\ui\\ready.png");
	Load<ATexture>(L"set", L"Resources\\ui\\set.png");
	Load<ATexture>(L"go", L"Resources\\ui\\go.png");
	Load<ATexture>(L"notes", L"Resources\\ui\\notes.png");

	Load<ATexture>(L"effectbluehold", L"Resources\\Effects\\holdCoverBlue.png");
	Load<ATexture>(L"effectgreenhold", L"Resources\\Effects\\holdCoverGreen.png");
	Load<ATexture>(L"effectpurplehold", L"Resources\\Effects\\holdCoverPurple.png");
	Load<ATexture>(L"effectredhold", L"Resources\\Effects\\holdCoverRed.png");

	Load<ATexture>(L"tapEffects", L"Resources\\Effects\\noteSplashes.png");

	Load<ATexture>(L"NoteHoldBar", L"Resources\\ui\\NOTE_hold_assets.png");

	wstring comboNumName = L"num";
	
	for (UINT i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"num%d", i);

		Load<ATexture>(Buff, L"Resources\\ui\\" + wstring(Buff) + L".png");
	}

	Load<ATexture>(L"combo", L"Resources\\ui\\combo.png");
	Load<ATexture>(L"bad", L"Resources\\ui\\bad.png");
	Load<ATexture>(L"good", L"Resources\\ui\\good.png");
	Load<ATexture>(L"sick", L"Resources\\ui\\sick.png");
	Load<ATexture>(L"shit", L"Resources\\ui\\shit.png");

	Load<ATexture>(L"gftitle", L"Resources\\mainUI\\gfDanceTitle.png");
	Load<ATexture>(L"logotitle", L"Resources\\mainUI\\logoBumpin.png");
	Load<ATexture>(L"titleenter", L"Resources\\mainUI\\titleEnter.png");

	Load<ATexture>(L"loading", L"Resources\\ui\\Loading.png");


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
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"GF");
	pMtrl->SetShader(Find<AGraphicShader>(L"AtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"gf"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"StageBack");
	pMtrl->SetShader(Find<AGraphicShader>(L"Std2DLightShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"stageback"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"StageFront");
	pMtrl->SetShader(Find<AGraphicShader>(L"Std2DLightShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"stagefront"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"NoteReceptor");
	pMtrl->SetShader(Find<AGraphicShader>(L"UIAtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"notereceptor"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"NoteBar");
	pMtrl->SetShader(Find<AGraphicShader>(L"NoteBarShader"));
	//pMtrl->SetTexture(TEX_1, Find<ATexture>(L"notes"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	wstring path = CONTENT_PATH;
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Notes");
	pMtrl->SetShader(Find<AGraphicShader>(L"NoteShader"));
	//pMtrl->SetTexture(TEX_0, Find<ATexture>(L"notes"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Daddy");
	pMtrl->SetShader(Find<AGraphicShader>(L"AtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"daddy"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	// 왜 여기에 Domain 하면 엄청난 누수가 발생하는가
	pMtrl = new AMaterial;
	pMtrl->SetName(L"RSG");
	pMtrl->SetShader(Find<AGraphicShader>(L"RSGShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"ready"));
	pMtrl->SetTexture(TEX_1, Find<ATexture>(L"set"));
	pMtrl->SetTexture(TEX_2, Find<ATexture>(L"go"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	for (UINT i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"num%d", i);

		pMtrl = new AMaterial;
		pMtrl->SetName(Buff);
		pMtrl->SetShader(Find<AGraphicShader>(L"ComboShader"));
		pMtrl->SetTexture(TEX_0, Find<ATexture>(Buff));
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
		AddAsset(pMtrl->GetName(), pMtrl.Get());
		pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");
	}

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Bad");
	pMtrl->SetShader(Find<AGraphicShader>(L"ComboShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"bad"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Combo");
	pMtrl->SetShader(Find<AGraphicShader>(L"ComboShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"combo"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Good");
	pMtrl->SetShader(Find<AGraphicShader>(L"ComboShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"good"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Shit");
	pMtrl->SetShader(Find<AGraphicShader>(L"ComboShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"shit"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Sick");
	pMtrl->SetShader(Find<AGraphicShader>(L"ComboShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"sick"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	pMtrl = new AMaterial;
	pMtrl->SetName(L"gfTitle");
	pMtrl->SetShader(Find<AGraphicShader>(L"UIAtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"gftitle"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	pMtrl = new AMaterial;
	pMtrl->SetName(L"logoTitle");
	pMtrl->SetShader(Find<AGraphicShader>(L"UIAtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"logotitle"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	pMtrl = new AMaterial;
	pMtrl->SetName(L"titleEnter");
	pMtrl->SetShader(Find<AGraphicShader>(L"UIAtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"titleenter"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	pMtrl = new AMaterial;
	pMtrl->SetName(L"titleBlink");
	pMtrl->SetShader(Find<AGraphicShader>(L"TitleBlinkShader"));
	pMtrl->SetScalar(SCALAR_PARAM::INT_0, -1);
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	// Tap Effects
	pMtrl = new AMaterial;
	pMtrl->SetName(L"tapEffects");
	pMtrl->SetShader(Find<AGraphicShader>(L"AtlasUVShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"tapEffects"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());
	pMtrl->Save(path + L"Material\\" + pMtrl->GetName() + L".mtrl");

	wstring tags[3] = { L"Tap", L"Pressed", L"Released" };

	for (int i = 0; i < 3; i++)
	{
		// Hold Effects
		pMtrl = new AMaterial;
		pMtrl->SetName(L"EffectBlueHold" + tags[i]);
		pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
		pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectbluehold"));
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
		AddAsset(pMtrl->GetName(), pMtrl.Get());

		pMtrl = new AMaterial;
		pMtrl->SetName(L"EffectGreenHold" + tags[i]);
		pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
		pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectgreenhold"));
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
		AddAsset(pMtrl->GetName(), pMtrl.Get());

		pMtrl = new AMaterial;
		pMtrl->SetName(L"EffectPurpleHold" + tags[i]);
		pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
		pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectpurplehold"));
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
		AddAsset(pMtrl->GetName(), pMtrl.Get());

		pMtrl = new AMaterial;
		pMtrl->SetName(L"EffectRedHold" + tags[i]);
		pMtrl->SetShader(Find<AGraphicShader>(L"VolatileShader"));
		pMtrl->SetTexture(TEX_0, Find<ATexture>(L"effectredhold"));
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
		AddAsset(pMtrl->GetName(), pMtrl.Get());
	}

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Loading");
	pMtrl->SetShader(Find<AGraphicShader>(L"Std2DShader"));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"loading"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_OPAQUE);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	// =========
	// Std2DMtrl 
	// =========
	pMtrl = new AMaterial;
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(Find<AGraphicShader>(L"Std2DShader"));
	// Parameter
	pMtrl->SetScalar(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
	pMtrl->SetTexture(TEX_0, Find<ATexture>(L"Fighter"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());


	// =======
	// DbgMtrl 
	// =======
	pMtrl = new AMaterial;
	pMtrl->SetName(L"DbgMtrl");
	pMtrl->SetShader(Find<AGraphicShader>(L"DbgShader"));
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_DEBUG);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	//Load<AMaterial>(L"Material\\Default Material_0.mtrl", L"Material\\Default Material_0.mtrl");
}

void AssetManager::CreateEngineSprite()
{
/*	Ptr<ATexture> pAtlas = FIND(ATexture, L"stageback");*/

}