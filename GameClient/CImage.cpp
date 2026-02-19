#include "pch.h"
#include "CImage.h"

CImage::CImage()
{
}

CImage::~CImage()
{
}

void CImage::Tick()
{

}

void CImage::SetTexture(Ptr<ATexture> tex)
{
	m_imgTexture = tex;
}
