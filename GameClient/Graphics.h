#pragma once
#include "ConstantBuffer.h"

class Graphics : public SingletonClass<Graphics>
{
	SINGLE(Graphics);
public:
	// device setting
	void Init(HWND hwnd);
	void Present() { m_swapchain->Present(0, 0); }

public:
	ComPtr<ID3D11Device> GetDevice() const { return m_device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return m_context; }
	ComPtr<ID3D11RenderTargetView> GetRTV() const { return m_rtv; }
	ComPtr<ID3D11DepthStencilView> GetDSV() const { return m_dsv; }                                  

	Ptr<ConstantBuffer> GetConstantBuffer(CB_TYPE type) { return m_cbuffer[(UINT)type]; }

	void ClearView();
	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE type) { return m_rsState[(UINT)type]; }
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE type) { return m_blendState[(UINT)type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_dsState[(UINT)_Type]; }

	void OMSetRTV();

private:
	void CreateDeviceAndSwapChain(HWND hwnd);
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void CreateConstantBuffer();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSamplerState();

private:
	ComPtr<ID3D11Device>			m_device;			// GPU 메모리 관리
	ComPtr<ID3D11DeviceContext>		m_context;			// GPU 제어 및 렌더링 명령, GPGPU(General Purpose) - Compute Shader
	
	ComPtr<IDXGISwapChain>			m_swapchain;		// 

	ComPtr<ID3D11Texture2D>			m_rtvTexture;
	ComPtr<ID3D11RenderTargetView>	m_rtv;

	ComPtr<ID3D11Texture2D>			m_dsTexture;
	ComPtr<ID3D11DepthStencilView>	m_dsv;

	Ptr<ConstantBuffer>				m_cbuffer[(UINT)CB_TYPE::END];
	
	ComPtr<ID3D11RasterizerState>	m_rsState[(UINT)RS_TYPE::END];
	ComPtr<ID3D11BlendState>		m_blendState[UINT(BS_TYPE::END)];
	ComPtr<ID3D11DepthStencilState>	m_dsState[(UINT)DS_TYPE::END];

	ComPtr<ID3D11SamplerState>		m_samplerState[2];
};

