#pragma once
#include "CScript.h"

class CGameManager;
class CEventManager;

/// <summary>
/// 모든 맵의 시작을 알리는 클래스.
/// Ready-Set-Go 텍스처 출력 후 끝나는 타이밍에 노래가 시작하도록 해주기
/// </summary>
class CComboImage : public CScript
{
public:
	CComboImage();
	virtual ~CComboImage();

public:
	virtual void Begin() override;
	virtual void Tick() override;

	CLONE(CComboImage);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	Vec3 m_destinaion;

	float m_elapsed;
	float m_duration;
};

