#pragma once
#include "Entity.h"
class ConstantBuffer : public Entity
{
public:
	ConstantBuffer();
	virtual ~ConstantBuffer();

public:
	int Create(CB_TYPE type, UINT size);
	void SetData(void* sysMem, UINT size = 0);
	void Binding();

private:
	ComPtr<ID3D11Buffer> m_cBuffer;
	D3D11_BUFFER_DESC m_desc;
	CB_TYPE m_type;

};

