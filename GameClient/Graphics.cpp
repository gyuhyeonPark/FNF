#include "pch.h"
#include "Graphics.h"

Graphics::Graphics()
{
}

void Graphics::Init(HWND hwnd)
{
	// 파이프라인 등록 전, 필요한 d3d 객체들을 제작해준다.
	CreateDeviceAndSwapChain(hwnd);
	CreateRenderTargetView();
	CreateDepthStencilView();
	CreateConstantBuffer();

	HRESULT hr;

	D3D11_VIEWPORT viewPort;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = static_cast<float>(GWinSizeX);
	viewPort.Height = static_cast<float>(GWinSizeY);
	g_Global.Resolution = Vec2(GWinSizeX, GWinSizeY);
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	m_context->RSSetViewports(1, &viewPort);
	(CreateBlendState());
	(CreateRasterizerState());
	(CreateDepthStencilState());
	(CreateSamplerState());

	m_context->OMSetRenderTargets(
		1, m_rtv.GetAddressOf(), m_dsv.Get());
}

void Graphics::CreateDeviceAndSwapChain(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = GWinSizeX;
		desc.BufferDesc.Height = GWinSizeY;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = hwnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT flags = 0;

#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		m_swapchain.GetAddressOf(),
		m_device.GetAddressOf(),
		nullptr,
		m_context.GetAddressOf()
	);
	CHECK(hr);
}

void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_rtvTexture.GetAddressOf());
	CHECK(hr);

	hr = m_device->CreateRenderTargetView(m_rtvTexture.Get(), nullptr, m_rtv.GetAddressOf());
	CHECK(hr);
}

void Graphics::CreateDepthStencilView()
{
	{
		// depth-stencil Target 생성
		D3D11_TEXTURE2D_DESC desc = { 0 };
		ZeroMemory(&desc, sizeof(desc));

		// 해상도 통일
		desc.Width = static_cast<uint32>(GWinSizeX);
		desc.Height = static_cast<uint32>(GWinSizeY);
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		desc.MipLevels = 1;		// 밉맵 : 저화질 버전 텍스처 추가 생성. 1 : 원본
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		
		// CPU 접근 제한
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		
		desc.MiscFlags = 0;

		// 2번째 인자 subresource?
		// 
		HRESULT hr = m_device->CreateTexture2D(&desc, nullptr, m_dsTexture.GetAddressOf());
		CHECK(hr);
	}

	{
		// Depth-Stencil View 생성
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = m_device->CreateDepthStencilView(m_dsTexture.Get(), &desc, m_dsv.GetAddressOf());
		CHECK(hr);
	}
}

void Graphics::CreateConstantBuffer()
{
	// 앞으로 사용할 상수버퍼 미리 생성	
	m_cbuffer[(UINT)CB_TYPE::TRANSFORM] = new ConstantBuffer;
	m_cbuffer[(UINT)CB_TYPE::TRANSFORM]->Create(CB_TYPE::TRANSFORM, sizeof(ConstantStruct));
	
	m_cbuffer[(UINT)CB_TYPE::MATERIAL] = new ConstantBuffer;
	m_cbuffer[(UINT)CB_TYPE::MATERIAL]->Create(CB_TYPE::MATERIAL, sizeof(MtrlConst));

	m_cbuffer[(UINT)CB_TYPE::GLOBAL] = new ConstantBuffer;
	m_cbuffer[(UINT)CB_TYPE::GLOBAL]->Create(CB_TYPE::GLOBAL, sizeof(GlobalData));
}

int Graphics::CreateRasterizerState()
{
	// CULL_BACK
	m_rsState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	// CULL_FRONT
	D3D11_RASTERIZER_DESC Desc = {};
	Desc.CullMode = D3D11_CULL_FRONT;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_rsState[(UINT)RS_TYPE:: CULL_FRONT].GetAddressOf());

	// CULL_NONE
	Desc = {};
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_rsState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

	// WIRE_FRAME
	Desc = {};
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_WIREFRAME;
	DEVICE->CreateRasterizerState(&Desc, m_rsState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

	return S_OK;
}

int Graphics::CreateDepthStencilState()
{
	// Less
	m_dsState[(UINT)DS_TYPE::LESS] = nullptr;

	// LessEqual
	D3D11_DEPTH_STENCIL_DESC Desc = {};

	Desc.DepthEnable = true;
	Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;		// 작거나 같으면 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// 자신의 깊으로 덮어 씀
	Desc.StencilEnable = false;

	HRESULT hr;

	hr = m_device->CreateDepthStencilState(&Desc, m_dsState[(UINT)DS_TYPE::LESS_EQUL].GetAddressOf());
	CHECK(hr);

	// NO_TEST
	Desc.DepthEnable = true;
	Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;			// 깊이판정은 항상 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// 자신의 깊으로 덮어 씀
	Desc.StencilEnable = false;

	hr = m_device->CreateDepthStencilState(&Desc, m_dsState[(UINT)DS_TYPE::NO_TEST].GetAddressOf());
	CHECK(hr);

	// NO_TEST_NO_WRITE
	Desc.DepthEnable = false;
	hr = m_device->CreateDepthStencilState(&Desc, m_dsState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());
	CHECK(hr);

	// TRANSPARENT 전용
	Desc = {};

	Desc.DepthEnable = true;
	Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // 🔥 핵심!!
	Desc.StencilEnable = false;
	hr = m_device->CreateDepthStencilState(&Desc, m_dsState[(UINT)DS_TYPE::TRANS_PARENT].GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int Graphics::CreateBlendState()
{
	HRESULT hr = S_OK;

	// 1. Default. 기본적으로 제공되는 blend state.
	m_blendState[(UINT)BS_TYPE::DEFAULT] = nullptr;

	// 2. Alpha Blend : (SrcRGB * SrcA) + (DestRGB * (1 - SrcA))
	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	// 도출된 rgb의 픽셀의 alpha는 어떤 값으로 할당할 것인가? 중요 X
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	hr = m_device->CreateBlendState(&blendDesc, m_blendState[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf());
	CHECK(hr);

	// 3. One-One Blend : (SrcRGB * 1) + (DestRGB * 1)
	blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	// 도출된 rgb의 픽셀의 alpha는 어떤 값으로 할당할 것인가? 중요 X
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	hr = m_device->CreateBlendState(&blendDesc, m_blendState[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());
	CHECK(hr);
	return S_OK;
}

int Graphics::CreateSamplerState()
{
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;    
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = m_device->CreateSamplerState(&sampDesc, m_samplerState[0].GetAddressOf());
	CHECK(hr);

	// s0 레지스터 바인딩 (모든 파이프 라인)
	m_context->VSSetSamplers(0, 1, m_samplerState[0].GetAddressOf());
	m_context->HSSetSamplers(0, 1, m_samplerState[0].GetAddressOf());
	m_context->DSSetSamplers(0, 1, m_samplerState[0].GetAddressOf());
	m_context->GSSetSamplers(0, 1, m_samplerState[0].GetAddressOf());
	m_context->PSSetSamplers(0, 1, m_samplerState[0].GetAddressOf());

	// =====================
	// POINT 필터링 + 랩 모드
	// =====================
	sampDesc = {};
	
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	hr = m_device->CreateSamplerState(&sampDesc, m_samplerState[1].GetAddressOf());
	CHECK(hr);

	// s1 레지스터 바인딩 (모든 파이프 라인)
	m_context->VSSetSamplers(1, 1, m_samplerState[1].GetAddressOf());
	m_context->HSSetSamplers(1, 1, m_samplerState[1].GetAddressOf());
	m_context->DSSetSamplers(1, 1, m_samplerState[1].GetAddressOf());
	m_context->GSSetSamplers(1, 1, m_samplerState[1].GetAddressOf());
	m_context->PSSetSamplers(1, 1, m_samplerState[1].GetAddressOf());

	return hr;
}

void Graphics::OMSetRTV()
{
	m_context->OMSetRenderTargets(
		1, m_rtv.GetAddressOf(), m_dsv.Get());
}

void Graphics::ClearView()
{
	FLOAT clearColor[4] = { 0.f, 0.f, 0.f, 0.0f };

	m_context->ClearRenderTargetView(m_rtv.Get(), clearColor);
	m_context->ClearDepthStencilView(
		m_dsv.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
	);
}
