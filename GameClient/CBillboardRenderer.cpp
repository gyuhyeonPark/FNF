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
	if (nullptr == GetMesh() || nullptr == GetMtrl())
		return;

	GetMtrl()->SetScalar(VEC2_0, m_BillboardScale);
	GetMtrl()->Binding();

	GetMesh()->Render();

	GetMtrl()->Clear();
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

		// 찾는 쉐이더가 없으면 만들어서 에셋매니저에 등록해둔다
		if (nullptr == pShader)
		{
			pShader = new AGraphicShader;
			pShader->SetName(L"BillboardShader");
			pShader->CreateVertexShader(L"Shader\\billboard.hlsl", "VS_Billboard");
			pShader->CreatePixelShader(L"Shader\\billboard.hlsl", "PS_Billboard");
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
