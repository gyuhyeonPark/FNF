#include "pch.h"
#include "AMesh.h"

AMesh::AMesh()
    : Asset(ASSET_TYPE::MESH), m_vtxSys(nullptr), m_idxSys(nullptr)
{

}

AMesh::~AMesh()
{
	delete[] m_vtxSys;
	delete[] m_idxSys;
}

int AMesh::Create(VertexInfo* _vtxSysMem, UINT _vtxCount, UINT* _idxSysmem, UINT _idxCount)
{
	m_vtxCount = _vtxCount;
	m_vtxDesc.ByteWidth = sizeof(VertexInfo) * m_vtxCount;

	// vertext의 경우 GPU에 넘겨준 이후 Transform 변환 가능 -> CPU에서 접근 가능하도록 설정.
	m_vtxDesc.CPUAccessFlags = 0;
	m_vtxDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_vtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// CreateBuffer 시 최초로 전달할 수 있는 데이터.
	// pSystem 메모리에 정점 배열의 시작 주소를 전달한다.
	// description의 bytewidth와 시작 주소를 통해 GPU는 정확한 크기의 정점 배열을 읽어 들일 수 있게 된다.
	D3D11_SUBRESOURCE_DATA vData = {};
	vData.pSysMem = _vtxSysMem;

	HRESULT hr = DEVICE->CreateBuffer(&m_vtxDesc, &vData, m_vBuffer.GetAddressOf());
	CHECK(hr);

	m_vtxSys = new VertexInfo[_vtxCount];
	for (int i = 0; i < _vtxCount; i++)
	{
		m_vtxSys[i] = _vtxSysMem[i];
	}

	m_idxCount = _idxCount;
	m_idxDesc.Usage = D3D11_USAGE_DEFAULT;
	m_idxDesc.ByteWidth = sizeof(UINT) * m_idxCount;
	m_idxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA iData = {};
	iData.pSysMem = _idxSysmem;

	hr = DEVICE->CreateBuffer(&m_idxDesc, &iData, m_iBuffer.GetAddressOf());
	CHECK(hr);

	m_idxSys = new UINT[_idxCount];
	for (int i = 0; i < _idxCount; i++)
	{
		m_idxSys[i] = _idxSysmem[i];
	}

	return S_OK;
}

void AMesh::Render()
{
	UINT stride = sizeof(VertexInfo);
	UINT uvOffset = 0;

	DEVICE_CONTEXT->IASetVertexBuffers(
		0, 1, m_vBuffer.GetAddressOf(), &stride, &uvOffset);

	DEVICE_CONTEXT->IASetIndexBuffer(
		m_iBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	DEVICE_CONTEXT->DrawIndexed(m_idxCount, 0, 0);
}
