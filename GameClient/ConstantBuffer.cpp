#include "pch.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
	: m_desc{}, m_type(CB_TYPE::END)
{
}

ConstantBuffer::~ConstantBuffer()
{
}

int ConstantBuffer::Create(CB_TYPE type, UINT size)
{
	m_type = type;
	
	// uv
	m_desc.ByteWidth = size;
	m_desc.Usage = D3D11_USAGE_DYNAMIC;
	m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = DEVICE->CreateBuffer(&m_desc, nullptr, m_cBuffer.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

void ConstantBuffer::SetData(void* sysMem, UINT size)
{
	if (size == 0)
	{
		size = m_desc.ByteWidth;
	}

	D3D11_MAPPED_SUBRESOURCE sub = {};
	DEVICE_CONTEXT->Map(m_cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, sysMem, size);
	DEVICE_CONTEXT->Unmap(m_cBuffer.Get(), 0);
}

void ConstantBuffer::Binding()
{
	DEVICE_CONTEXT->VSSetConstantBuffers((UINT)m_type/*상수버퍼를 바인딩할 레지스터 번호*/, 1, m_cBuffer.GetAddressOf());
	DEVICE_CONTEXT->HSSetConstantBuffers((UINT)m_type/*상수버퍼를 바인딩할 레지스터 번호*/, 1, m_cBuffer.GetAddressOf());
	DEVICE_CONTEXT->DSSetConstantBuffers((UINT)m_type/*상수버퍼를 바인딩할 레지스터 번호*/, 1, m_cBuffer.GetAddressOf());
	DEVICE_CONTEXT->GSSetConstantBuffers((UINT)m_type/*상수버퍼를 바인딩할 레지스터 번호*/, 1, m_cBuffer.GetAddressOf());
	DEVICE_CONTEXT->PSSetConstantBuffers((UINT)m_type/*상수버퍼를 바인딩할 레지스터 번호*/, 1, m_cBuffer.GetAddressOf());
}
