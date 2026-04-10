#pragma once
#include "Asset.h"

enum class SHADER_PARAM
{
	INT,
	FLOAT,
	VEC2,
	VEC4,
	MAT,
	TEX,
};

struct ShaderParam
{
	SHADER_PARAM    Type;
	int             Index;
	wstring         Desc;
	int             Step;    // ImGui 에서 값의 변화량
	bool            IsInput; // ImGui 에서 표현할 위젯 스타일
};

/// <summary>
/// 렌더링 파이프라인 과정을 의미하는 클래스.
/// 여러 개의 물체들이 해당 파이프라인 과정을 공유하게 된다.
/// </summary>
class AGraphicShader : public Asset
{
public:
	AGraphicShader();
	virtual ~AGraphicShader();
	
public:
	void AddShaderParam(SHADER_PARAM _Type, int _Idx, const wstring& _Desc, int _Step = 0, bool IsInput = true)
	{
		m_vecShaderParam.push_back(ShaderParam{ _Type, _Idx, _Desc, _Step, IsInput });
	}
	const vector<ShaderParam>& GetShaderParam() { return m_vecShaderParam; }

public:
	int CreateVertexShader(const wstring& _relativeFilePath, const string& _funcName);
	int CreatePixelShader(const wstring& _relativeFilePath, const string& _funcName);
	void Binding();
	
	GET_SET(D3D11_PRIMITIVE_TOPOLOGY, Topology);
	GET_SET(RS_TYPE, RSType);
	GET_SET(DS_TYPE, DSType);
	GET_SET(BS_TYPE, BSType);

private:
	ComPtr<ID3DBlob> m_vsBlob;
	ComPtr<ID3DBlob> m_psBlob;

	ComPtr<ID3D11VertexShader>			m_vs;
	ComPtr<ID3D11PixelShader>			m_ps;
	ComPtr<ID3D11InputLayout>			m_inputLayout;
	D3D11_PRIMITIVE_TOPOLOGY			m_Topology;

	RS_TYPE m_RSType; // 레스터라이져 컬링모드
	DS_TYPE m_DSType;
	BS_TYPE m_BSType;

	vector<ShaderParam> m_vecShaderParam;
};

