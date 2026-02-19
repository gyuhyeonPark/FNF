#pragma once
#include "Asset.h"
/// <summary>
/// 
/// </summary>
class AMesh : public Asset
{
public:
	int Create(VertexInfo* _vtxSysMem, UINT _vtxCount, UINT* _idxSysmem, UINT _idxCount);
	void Render();

	const VertexInfo* GetVtxSysMem() { return m_vtxSys; }
	const UINT* GetIdxSysMem() { return m_idxSys; }

public:
	AMesh();
	virtual ~AMesh();

private:
	ComPtr<ID3D11Buffer> m_vBuffer;
	D3D11_BUFFER_DESC m_vtxDesc;
	UINT m_vtxCount;
	VertexInfo* m_vtxSys;

	ComPtr<ID3D11Buffer> m_iBuffer;
	D3D11_BUFFER_DESC m_idxDesc;
	UINT m_idxCount;
	UINT* m_idxSys;
};

