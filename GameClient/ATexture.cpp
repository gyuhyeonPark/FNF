#include "pch.h"
#include "ATexture.h"

ATexture::ATexture()
	: Asset(ASSET_TYPE::TEXTURE), m_recentNum(-1)
{
}

ATexture::~ATexture()
{
}

int ATexture::Load(const wstring& _filePath)
{
	/*
	wchar_t szDrive[10] = {};
	wchar_t szFolder[255] = {};
	wchar_t szFileName[50] = {};
	wchar_t szExt[10] = {};
	_wsplitpath_s(_strFilePath.c_str(), szDrive, 10, szFolder, 255, szFileName, 50, szExt, 10);
	*/

	wchar_t szExt[10] = {};
	_wsplitpath_s(_filePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 10);
	wstring strExt = szExt;

	HRESULT hr = S_OK;

	// .dds
	if (L".dds" == strExt)
	{
		hr = LoadFromDDSFile(_filePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}
	// .tga
	else if (L".tga" == strExt)
	{
		hr = LoadFromTGAFile(_filePath.c_str(), nullptr, m_Image);
	}
	// WIC(Window Image Component) .png, .jpg, .jpeg, .bmp
	else
	{
		hr = LoadFromWICFile(_filePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	} 

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"텍스쳐 시스템메모리 로딩 실패", L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	// SysMem			->	GPU
	// ScratchImage		->	Texture2D	
	// Texture2D 생성

	// Texture2D -> RTV  ->
	//           -> DSV  ->
	//           -> SRV  ->
	// View 생성

	// ScratcgImage 에 로딩된 이미지 데이터를 기반으로 Texture2D 를 생성하고, 
	// 다시 이걸로 ShaderResourceView 까지 만들어서 ShaderResourceView 주소를 알려줌
	if (FAILED(CreateShaderResourceView(DEVICE.Get(), m_Image.GetImages()
		, m_Image.GetImageCount(), m_Image.GetMetadata()
		, m_srv.GetAddressOf())))
	{
		MessageBox(nullptr, L"ShaderResourveView 생성 실패", L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	// 생성된 SRV 를 이용해서, 먼저 만들어진 Texture2D 의 주소를 알아냄
	m_srv->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// Texture2D 를 생성할때 세팅한 Desc 옵션정보를 알아냄
	m_Tex2D->GetDesc(&m_Desc);

	m_width = m_Image.GetImage(0, 0, 0)->width;
	m_height = m_Image.GetImage(0, 0, 0)->height;

	return S_OK;
}

void ATexture::Binding(UINT registerNum)
{
	m_recentNum = registerNum;

	DEVICE_CONTEXT->VSSetShaderResources(m_recentNum, 1, m_srv.GetAddressOf());
	DEVICE_CONTEXT->GSSetShaderResources(m_recentNum, 1, m_srv.GetAddressOf());
	DEVICE_CONTEXT->HSSetShaderResources(m_recentNum, 1, m_srv.GetAddressOf());
	DEVICE_CONTEXT->DSSetShaderResources(m_recentNum, 1, m_srv.GetAddressOf());
	DEVICE_CONTEXT->PSSetShaderResources(m_recentNum, 1, m_srv.GetAddressOf());
}

void ATexture::Clear()
{
	if (-1 == m_recentNum)
		return;

	ID3D11ShaderResourceView* pSRV = nullptr;

	DEVICE_CONTEXT->VSSetShaderResources(m_recentNum, 1, &pSRV);
	DEVICE_CONTEXT->GSSetShaderResources(m_recentNum, 1, &pSRV);
	DEVICE_CONTEXT->HSSetShaderResources(m_recentNum, 1, &pSRV);
	DEVICE_CONTEXT->DSSetShaderResources(m_recentNum, 1, &pSRV);
	DEVICE_CONTEXT->PSSetShaderResources(m_recentNum, 1, &pSRV);

	m_recentNum = -1;
}
