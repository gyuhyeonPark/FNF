#include "pch.h"
#include "CSpriteRenderer.h"
#include "AssetManager.h"

CSpriteRenderer::CSpriteRenderer()
	: CRenderComponent(COMPONENT_TYPE::SPRITE_RENDERER)
{
}

CSpriteRenderer::~CSpriteRenderer()
{
}

void CSpriteRenderer::FinalTick()
{
}

void CSpriteRenderer::Render()
{
	GetMtrl()->SetTexture(TEX_0, m_Sprite->GetAtlas());
	GetMtrl()->SetScalar(VEC2_0, m_Sprite->GetLeftTopUV());
	GetMtrl()->SetScalar(VEC2_1, m_Sprite->GetSliceUV());

	GetMtrl()->Binding();
	GetMesh()->Render();
}

void CSpriteRenderer::CreateMaterial()
{
	wstring MeshName = L"RectMesh";
	wstring MtrlName = L"SpriteMtrl";
	wstring ShaderName = L"SpriteShader";
	wstring FilePath = L"Shaders\\sprite.hlsl";
	string VS = "VS_Sprite";
	string PS = "PS_Sprite";

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

		// 찾는 쉐이더가 없으면 만들어서 에셋매니저에 등록해둔다
		if (nullptr == pShader)
		{
			pShader = new AGraphicShader;
			pShader->SetName(ShaderName);
			pShader->CreateVertexShader(FilePath, VS);
			pShader->CreatePixelShader(FilePath, PS);
			pShader->SetBSType(BS_TYPE::DEFAULT);
			pShader->SetRSType(RS_TYPE::CULL_NONE);
			AssetManager::GetInstance()->AddAsset(pShader->GetName(), pShader.Get());
		}

		// 찾은 or 생성한 쉐이더를 재질에 설정해주고, 재질도 에셋매니저에 등록한다.
		pMtrl->SetShader(pShader);
		AssetManager::GetInstance()->AddAsset(pMtrl->GetName(), pMtrl.Get());
	}

	SetMtrl(pMtrl);
}
