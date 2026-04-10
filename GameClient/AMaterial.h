#pragma once
#include "Asset.h"
#include "assets.h"

class AMaterial : public Asset
{
public:
	AMaterial();
    AMaterial(const AMaterial& _other);
	virtual ~AMaterial();

public:
	void SetShader(Ptr<AGraphicShader> shader) { m_shader = shader; }
	void SetTexture(TEX_PARAM param, Ptr<ATexture> tex) { m_tex[param] = tex; }
    Ptr<ATexture> GetTexture(TEX_PARAM _Param) { return m_tex[_Param]; }

    Ptr<AGraphicShader> GetShader() { return m_shader; }
    
    GET_SET(RENDER_DOMAIN, Domain);

    void Binding();

    template<typename T>
    void SetScalar(SCALAR_PARAM _Param, const T& _Data);

    template<typename T>
    T& GetScalar(SCALAR_PARAM _Param);

    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;

    AMaterial* Clone();

    void Clear();

    GET_SET(RENDER_DOMAIN, Domain);

private:
	Ptr<AGraphicShader> m_shader;
	Ptr<ATexture> m_tex[TEX_END];
	MtrlConst m_const;
    RENDER_DOMAIN m_Domain;
};

template<typename T>
void AMaterial::SetScalar(SCALAR_PARAM _Param, const T& _Data)
{
    switch (_Param)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3:
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, UINT>)
            m_const.iArr[_Param] = _Data;
        break;
    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3:
        if constexpr (std::is_same_v<T, float>)
            m_const.fArr[_Param - FLOAT_0] = _Data;
        break;
    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3:
        if constexpr (std::is_same_v<T, Vec2>)
            m_const.v2Arr[_Param - VEC2_0] = _Data;
        break;
    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3:
        if constexpr (std::is_same_v<T, Vec4>)
            m_const.v4Arr[_Param - VEC4_0] = _Data;
        break;
    case MAT_0:
    case MAT_1:
        if constexpr (std::is_same_v<T, Matrix>)
            m_const.mat[_Param - MAT_0] = _Data;
        break;
    }
}

template<typename T>
inline T& AMaterial::GetScalar(SCALAR_PARAM _Param)
{
    switch (_Param)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3:
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, UINT>)
            return m_const.iArr[_Param];
        break;
    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3:
        if constexpr (std::is_same_v<T, float>)
            return m_const.fArr[_Param - FLOAT_0];
        break;
    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3:
        if constexpr (std::is_same_v<T, Vec2>)
            return m_const.v2Arr[_Param - VEC2_0];
        break;
    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3:
        if constexpr (std::is_same_v<T, Vec4>)
            return m_const.v4Arr[_Param - VEC4_0];
        break;
    case MAT_0:
    case MAT_1:
        if constexpr (std::is_same_v<T, Matrix>)
            return m_const.mat[_Param - MAT_0];
        break;
    }
}
