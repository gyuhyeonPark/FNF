#pragma once
#include "Asset.h"

/// <summary>
/// 원하는 Mesh 객체에 원하는 Texture를 입혀주는 기능.
/// </summary>
class ATexture : public Asset
{
public:
	ATexture();
	virtual ~ATexture();

public:
	virtual int Load(const wstring& _filePath) override;
	void Binding(UINT registerNum);
	void Clear();

	size_t GetWidth() { return m_width; }
	size_t GetHeight() { return m_height; }

private:
	ComPtr<ID3D11ShaderResourceView>	m_srv;
	
	ScratchImage                        m_Image; // Content 폴더에 있는 이미지 파일을 메모리(SysMem) 로 불러드림
	ComPtr<ID3D11Texture2D>             m_Tex2D;		// SystemMem 로 로딩한 픽셀 데이터를 GPU 메모리로 전송
	D3D11_TEXTURE2D_DESC                m_Desc;

	int                                 m_recentNum;

	size_t m_width;
	size_t m_height;

	wstring m_fileName;

	friend class CAtlasAnimator;
	friend class CImage;
};

