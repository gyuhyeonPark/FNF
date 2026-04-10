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
	GetMaterial()->SetTexture(TEX_0, m_Sprite->GetAtlas());
	GetMaterial()->SetScalar(VEC2_0, m_Sprite->GetLeftTopUV());
	GetMaterial()->SetScalar(VEC2_1, m_Sprite->GetSliceUV());

	GetMaterial()->Binding();
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

		// 찾은 or 생성한 쉐이더를 재질에 설정해주고, 재질도 에셋매니저에 등록한다.
		pMtrl->SetShader(pShader);
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
		AssetManager::GetInstance()->AddAsset(pMtrl->GetName(), pMtrl.Get());
	}

	SetMaterial(pMtrl);
}

void CSpriteRenderer::SaveToSceneFile(FILE* _File)
{
}

void CSpriteRenderer::LoadFromSceneFile(FILE* _File)
{
}
