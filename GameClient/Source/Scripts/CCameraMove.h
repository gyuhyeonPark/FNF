#pragma once
#include "CScript.h"
class CCameraMove : public CScript
{
public:
	CCameraMove();
	virtual ~CCameraMove();

public:
	virtual void Begin() override;
	virtual void Tick() override;

private:
	void MovePerspective();
	void MoveOrthographic();

	void ZoomBeat();
	
public:
	void BeatHeat(float _zoomDuration, float _returnDuration, float _zoomRatio);

public:
	CLONE(CCameraMove);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	float m_speed;

	// zoom
	bool m_beatHeat;
	float m_elapsed;

	float m_zoomDuration;	// 확대까지 걸리는 시간
	float m_returnDuration;	// 돌아오는 데에 걸리는 시간

	float m_initZoomRatio;	// 초기 Orthoscale값
	float m_zoomRatio;		// 줌 배율
};

