#pragma once
#include "Easing.h"
#include "CScript.h"

/// <summary>
/// Dotween의 반복호출을 막기위한 클래스.
/// elasped? OnExpiriation?
/// CDotween 객체가 가지고 있으면 딱인 것 같다...
/// </summary>

class CDotweenTimer : public CScript
{
public:
	virtual void Tick() override;

	CLONE(CDotweenTimer);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	float m_xValue;
	float m_yValue;
	float m_zValue;
	float m_wValue;
	
	float easeTime;
	float elapsed;
	// 타이머가 시작된 후 경과된 시간
	float duration;
	float delay;

	// 현재 진행 거리
	float movingDistanceX;
	float movingDistanceY;
	float movingDistanceZ;
	float movingDistanceW;
	// 속도
	float speed;

	float normalizedDistanceX;
	float normalizedDistanceY;
	float normalizedDistanceZ;
	float normalizedDistanceW;

	float finalXvalue;
	float finalYvalue;
	float finalZvalue;
	float finalWvalue;

	float accumulated;		// 누적 회전량

	bool isActive;
	bool isRepeat;
	bool isDone;			// 타이머 Vector에서 빼도 되는지에 대한 메시지 전달용

	float m_localTimeScale;

	easing_functions m_ease;

	std::function<void()> onUpdate = []() {};
	std::function<void()> onCompleteFunction = []() {};		// duration 도달했을 때 실행시킬 함수.
	std::function<void()> onExpiration = []() {};			// dotween 내부에서 설정해줄 수 있는 실행 완료 시 호출 함수

	CDotweenTimer();
	virtual ~CDotweenTimer();
	void Start();
	void Update();

	void Init();

	friend class CDotween;
};

