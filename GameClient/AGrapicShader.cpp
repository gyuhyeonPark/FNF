#include "pch.h"
#include "AGrapicShader.h"

AGraphicShader::AGraphicShader()
	: Asset(ASSET_TYPE::GRAPHICSHADER), m_BSType(BS_TYPE::DEFAULT),
	m_RSType(RS_TYPE::CULL_NONE), m_DSType(DS_TYPE::LESS),
	m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

AGraphicShader::~AGraphicShader()
{
}

int AGraphicShader::CreateVertexShader(const wstring& _relativeFilePath, const string& _funcName)
{
	ID3DBlob* errorBlob = nullptr;

	wstring path = PathManager::GetInstance()->GetContentsPath() + _relativeFilePath;

	// VS
	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_funcName.c_str(),
		"vs_5_0",
		0, 0,
		&m_vsBlob,
		&errorBlob
	);
	CHECK(hr);

	// Shader 객체 생성
	hr = DEVICE->CreateVertexShader(
		m_vsBlob->GetBufferPointer(),
		m_vsBlob->GetBufferSize(),
		nullptr,
		m_vs.GetAddressOf()
	);
	CHECK(hr);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = DEVICE->CreateInputLayout(
		layout,
		3,
		m_vsBlob->GetBufferPointer(),
		m_vsBlob->GetBufferSize(),
		m_inputLayout.GetAddressOf()
	);
	CHECK(hr);

	return hr;
}

int AGraphicShader::CreatePixelShader(const wstring& _relativeFilePath, const string& _funcName)
{
	ID3DBlob* errorBlob = nullptr;

	wstring path = PathManager::GetInstance()->GetContentsPath() + _relativeFilePath;

	// PS
	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_funcName.c_str(),
		"ps_5_0",
		0, 0,
		&m_psBlob,
		&errorBlob
	);
	CHECK(hr);

	hr = DEVICE->CreatePixelShader(
		m_psBlob->GetBufferPointer(),
		m_psBlob->GetBufferSize(),
		nullptr,
		m_ps.GetAddressOf()
	);
	CHECK(hr);

	return S_OK;
}

void AGraphicShader::Binding()
{
	if (GetKey() == L"TitleBlinkShader")
	{
		int a = 0;
	}
	
	DEVICE_CONTEXT->VSSetShader(m_vs.Get(), nullptr, 0);
	DEVICE_CONTEXT->PSSetShader(m_ps.Get(), nullptr, 0);
	DEVICE_CONTEXT->IASetInputLayout(m_inputLayout.Get());
	DEVICE_CONTEXT->IASetPrimitiveTopology(m_Topology);

	// Rasterizer State	
	DEVICE_CONTEXT->RSSetState(Graphics::GetInstance()->GetRSState(m_RSType).Get());

	ComPtr<ID3D11BlendState> bsState = Graphics::GetInstance()->GetBSState(m_BSType);
	float blendFactor[4] = { 0,0,0,0 };
	DEVICE_CONTEXT->OMSetBlendState(bsState.Get(), blendFactor, 0xffffffff);
}
