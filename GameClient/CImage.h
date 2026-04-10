#pragma once
#include "CScript.h"
/// <summary>
/// ATexture 에셋을 오브젝트에 추가, 이미지를 화면에 띄워준다
/// </summary>
class CImage : public CScript
{
public:
	CImage();
	~CImage();

public:
	virtual void Tick() override;

	void SetTexture(Ptr<ATexture> tex);

private:
	Ptr<ATexture> m_imgTexture;
};

