#include "pch.h"
#include "CBillboardRenderer.h"
#include "AssetManager.h"

CBillboardRenderer::CBillboardRenderer()
	: CRenderComponent(COMPONENT_TYPE::BILLBOARD_RENDER)
{
}

CBillboardRenderer::~CBillboardRenderer()
{
}

void CBillboardRenderer::FinalTick()
{
}

void CBillboardRenderer::Render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	GetMaterial()->SetScalar(VEC2_0, m_BillboardScale);
	GetMaterial()->Binding();

	GetMesh()->Render();

	GetMaterial()->Clear();
}

void CBillboardRenderer::CreateMaterial()
{
	// RectMesh 설정
	SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));

	// 재질 생성
	Ptr<AMaterial> pMtrl = AssetManager::GetInstance()->Find<AMaterial>(L"BillboardMtrl");

	// 찾는 재질이 없으면 생성한다.
	if (nullptr == pMtrl)
	{
		pMtrl = new AMaterial;
		pMtrl->SetName(L"BillboardMtrl");

		// 쉐이더를 찾아서 재질에 세팅해준다.
		Ptr<AGraphicShader> pShader = AssetManager::GetInstance()->Find<AGraphicShader>(L"BillboardShader");

		// 찾은 or 생성한 쉐이더를 재질에 설정해주고, 재질도 에셋매니저에 등록한다.
		pMtrl->SetShader(pShader);
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_OPAQUE);
		AssetManager::GetInstance()->AddAsset(pMtrl->GetName(), pMtrl.Get());
	}

	SetMaterial(pMtrl);
}

void CBillboardRenderer::SaveToSceneFile(FILE* _File)
{
	CRenderComponent::SaveToSceneFile(_File);

	fwrite(&m_BillboardScale, sizeof(Vec2), 1, _File);
	SaveAssetRef(_File, m_tex.Get());
}

void CBillboardRenderer::LoadFromSceneFile(FILE* _File)
{
	CRenderComponent::LoadFromSceneFile(_File);

	fread(&m_BillboardScale, sizeof(Vec2), 1, _File);
	m_tex = LoadAssetRef<ATexture>(_File);
}
