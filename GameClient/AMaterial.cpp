#include "pch.h"
#include "AMaterial.h"
#include "Graphics.h"
#include "AssetManager.h"

AMaterial::AMaterial()
	:Asset(ASSET_TYPE::MATERIAL), m_const{}
	, m_Domain(RENDER_DOMAIN::DOMAIN_NONE)
{
}

AMaterial::AMaterial(const AMaterial& _other)
	: Asset(_other), m_shader(_other.m_shader)
	, m_tex{}, m_const(_other.m_const), m_Domain(_other.m_Domain)
{
	for (int i = 0; i < TEX_END; ++i)
		m_tex[i] = _other.m_tex[i];
}

AMaterial::~AMaterial()
{
}

void AMaterial::Binding()
{
	m_shader->Binding();

	for (UINT i = 0; i < TEX_END; i++)
	{
		if (m_tex[i] == nullptr)
		{
			m_const.IsTex[i] = 0;
			continue;
		}
			
		m_tex[i]->Binding(i);
		m_const.IsTex[0] = 1;
	}
	Graphics::GetInstance()->GetConstantBuffer(CB_TYPE::MATERIAL)->SetData(&m_const);
	Graphics::GetInstance()->GetConstantBuffer(CB_TYPE::MATERIAL)->Binding();
}

int AMaterial::Save(const wstring& _FilePath)
{
	// 파일 스트림 커널
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");

	// 재질이 사용하는 쉐이더 파이프라인 정보
	SaveAssetRef(pFile, m_shader.Get());

	// 파이프라인 동작 시, 어떤 텍스처를 전달하기로 했었는지
	for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
	{
		SaveAssetRef(pFile, m_tex[i].Get());
	}

	// 파이프 라인 동작 시, 전달할 상수 데이터
	fwrite(&m_const, sizeof(MtrlConst), 1, pFile);

	// 렌더링 시점, 도메인
	fwrite(&m_Domain, sizeof(RENDER_DOMAIN), 1, pFile);

	fclose(pFile);

	return 0;
}

int AMaterial::Load(const wstring& _FilePath)
{
	// 파일 스트림 커널
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	// 재질이 사용하는 쉐이더 파이프라인 정보
	m_shader = LoadAssetRef<AGraphicShader>(pFile);

	// 파이프라인 동작 시, 어떤 텍스처를 전달하기로 했었는지
	for (UINT i = 0; i < TEX_END; ++i)
	{
		m_tex[i] = LoadAssetRef<ATexture>(pFile);
	}

	// 파이프라인 동작 시, 전달할 상수 데이터
	fread(&m_const, sizeof(MtrlConst), 1, pFile);

	// 렌더링 시점, 도메인
	fread(&m_Domain, sizeof(RENDER_DOMAIN), 1, pFile);

	fclose(pFile);

	return 0;
}

AMaterial* AMaterial::Clone()
{
	return new AMaterial(*this);
}

void AMaterial::Clear()
{
	for (int i = 0; i < TEX_END; ++i)
	{
		if (nullptr == m_tex[i])
			continue;

		m_tex[i]->Clear();
	}
}