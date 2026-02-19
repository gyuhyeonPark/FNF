#include "pch.h"
#include "AMaterial.h"
#include "Graphics.h"

AMaterial::AMaterial()
	:Asset(ASSET_TYPE::MATERIAL), m_const{}
{
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
			continue;

		m_tex[i]->Binding(i);
	}
	Graphics::GetInstance()->GetConstantBuffer(CB_TYPE::MATERIAL)->SetData(&m_const);
	Graphics::GetInstance()->GetConstantBuffer(CB_TYPE::MATERIAL)->Binding();
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