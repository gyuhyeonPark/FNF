#pragma once
#include "Component.h"

class CTransform : public Component
{
public:
	CTransform();
	virtual ~CTransform();

public:
	Vec3 GetRelativePosition() const { return m_relativePos; }
	Vec3 GetRelativeScale() const { return m_relativeScale; }
	Vec3 GetRelativeRotation() const { return m_relativeRot; }

	void SetRelativePosition(Vec3 pos) { m_relativePos = pos; FinalTick(); }
	void SetRelativeScale(Vec3 scale) { m_relativeScale = scale; FinalTick(); }
	void SetRelativeRotation(Vec3 rot) { m_relativeRot = rot; FinalTick(); }

	Vec3 GetWorldPos() { return m_matWorld.Translation(); }
	Vec3 GetWorldScale();

	void SetIndependentScale(bool boolen) { m_independentScale = boolen; }

	Vec3 GetDir(DIR director) { return m_dir[(UINT)director]; }
	const Matrix& GetWorldMat() { return m_matWorld; }
	void SetWorldMat(const Matrix& mat) { m_matWorld = mat; }

	virtual void FinalTick() override;

	// TEMP
	Vec4 GetUV() const { return m_uv; }
	void SetUV(Vec4 uv) { m_uv = uv; }
	void SetUVRot(bool boolen) { m_isRotated = boolen; }

public:
	void Binding();

	CLONE(CTransform);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	Vec3 m_relativePos;
	Vec3 m_relativeScale;
	Vec3 m_relativeRot;
	// 길이가 1인 단위벡터로 표현
	Vec3 m_dir[(UINT)DIR::END];

	Matrix m_matWorld;
	bool        m_independentScale; // 부모 오브젝트의 크기는 무시
	
	Vec4 m_uv;
	bool m_isRotated;
};

