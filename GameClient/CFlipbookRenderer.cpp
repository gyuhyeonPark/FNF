#include "pch.h"
#include "CFlipbookRenderer.h"
#include "AssetManager.h"

CFlipbookRenderer::CFlipbookRenderer()
	: CRenderComponent(COMPONENT_TYPE::FLIPBOOK_RENDERER), m_FPS(0.f), 
	m_AccTime(0.f), m_CurSprite(0)
{
}

CFlipbookRenderer::~CFlipbookRenderer()
{
}

void CFlipbookRenderer::FinalTick()
{
	if (CheckFinish())
		return;

	float fLmit = 1.f / m_FPS;
	m_AccTime += DT;

	if (fLmit < m_AccTime)
	{
		m_AccTime -= fLmit;
		++m_CurSprite;

		if (m_vecFlipbook[m_CurFlipbook]->GetSpriteCount() <= m_CurSprite)
		{
			m_Finish = true;
			--m_CurSprite;
		}
	}
}

void CFlipbookRenderer::Render()
{
	Ptr<AFlipbook> pCurFlipbook = m_vecFlipbook[m_CurFlipbook];
	Ptr<ASprite> pCurSprite = pCurFlipbook->GetSprite(m_CurSprite);

	GetMaterial()->SetTexture(TEX_0, pCurSprite->GetAtlas());
	GetMaterial()->SetScalar(VEC2_0, pCurSprite->GetLeftTopUV());
	GetMaterial()->SetScalar(VEC2_1, pCurSprite->GetSliceUV());

	GetMaterial()->Binding();
	GetMesh()->Render();
}

void CFlipbookRenderer::CreateMaterial()
{
	wstring MeshName = L"RectMesh";
	wstring MtrlName = L"SpriteMtrl";
	wstring ShaderName = L"SpriteShader";
	wstring FilePath = L"shaders\\flipbook.hlsl";
	string VS = "VS_Flipbook";
	string PS = "PS_Flipbook";

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

bool CFlipbookRenderer::CheckFinish()
{
	if (m_Finish)
	{
		if (0 < m_RepeatCount)
		{
			m_CurSprite = 0;
			m_Finish = false;
			--m_RepeatCount;
			return false;
		}
		else if (-1 == m_RepeatCount)
		{
			m_CurSprite = 0;
			m_Finish = false;
			return false;
		}
		else
		{
			return true;
		}
	}
}

void CFlipbookRenderer::SaveToSceneFile(FILE* _File)
{
	CRenderComponent::SaveToSceneFile(_File);

	size_t FlipbookCount = m_vecFlipbook.size();
	fwrite(&FlipbookCount, sizeof(size_t), 1, _File);

	for (const auto& Flipbook : m_vecFlipbook)
	{
		SaveAssetRef(_File, Flipbook.Get());
	}

	fwrite(&m_CurFlipbook, sizeof(int), 1, _File);
	fwrite(&m_CurSprite, sizeof(int), 1, _File);
	fwrite(&m_FPS, sizeof(int), 1, _File);
}

void CFlipbookRenderer::LoadFromSceneFile(FILE* _File)
{
	CRenderComponent::LoadFromSceneFile(_File);

	size_t FlipbookCount = 0;
	fread(&FlipbookCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < FlipbookCount; ++i)
	{
		m_vecFlipbook.push_back(LoadAssetRef<AFlipbook>(_File));
	}

	fread(&m_CurFlipbook, sizeof(int), 1, _File);
	fread(&m_CurSprite, sizeof(int), 1, _File);
	fread(&m_FPS, sizeof(int), 1, _File);
}
