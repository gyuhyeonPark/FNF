#pragma once
/// <summary>
/// Dotween을 이용하면 수치의 변경을 그래프대로 조절 가능.
/// 또한 delay를 이용해 n초 뒤에 실행시킬 수도 있도록 하기 (디폴트 매개변수)
/// 이를 이용한 position, scale, rotation 기능 구현.
/// 원하는 형태 : (GameObject) dotWeen->AddComponent<Dotween>;
/// dotWeen->DoMove( 대상 오브젝트, endValue, duration, delay, Ease )...
/// 매개변수가 너무 너무 많앙
/// 멤버변수로 Ease값 갖고, delay는 디폴트라고 치면...
/// 일단 delay = 0, Ease : Linear로 만들어보고 생각하자
/// 자기 자신을 계속 리턴해주면서 object의 값을 바꿔준다...?
/// </summary>
#include "Easing.h"
#include "CScript.h"

class CDotweenTimer;

enum class TimerIndex
{
	MoveTimer,
	RotateTimer,
	ScaleTimer,
	CustomTimer,
	NothingTimer,
	ShakeTimer,
	End
};

class CDotween : public CScript
{
public:
	CDotween();
	virtual ~CDotween();

public:
	virtual void Tick() override;

	void StopAllDotweenFunction();

	/// 첫 매개변수는 GameObject로 바꿔줄 예정...!
	Ptr<CDotween> DOMove(Vec3 endPosition, double p_duration);
	Ptr<CDotween> DOScale(Vec3 endScale, double p_duration);
	Ptr<CDotween> DORotate(Vec3 endRotation, double p_duration);
	Ptr<CDotween> DOQRotate(Vec3 axis, double angle, double p_duration);

	// DOLookAtForBoss은 Y축을 고려하지 않고 만든 것. (위아래 고려 X), DOLookAt은 Y축까지 돌린다.
	Ptr<CDotween> DOLookAt(Vec3 lookTransform, double duration, bool isYaxisInclude);

	// 아래의 함수들은 기존 Dotween엔 없는 함수들...
	Ptr<CDotween> DOCustom(double p_duration);
	Ptr<CDotween> DONothing(double p_duration);

	// strength : 축마다 이동하는 값.			vibrato : 흔들림의 강도
	Ptr<CDotween> DOShakePosition(double p_duration, double strength = 2.0f, int vibrato = 2);

	Ptr<CDotween> SetDelay(double p_delay);
	Ptr<CDotween> IsRepeat(bool p_repeat);
	Ptr<CDotween> SetEase(easing_functions p_ease);
	Ptr<CDotween> OnComplete(std::function<void()> expirationFunc);

	Ptr<CDotween> OnUpdate(std::function<void()> updateFunc);

	// DO 함수가 호출될 때마다 function을 넣어주기 위한 임시 객체.
	// 어차피 대입될 객체 delete 될 것이기 때문에 따로 delete 하지 않는다.

	// 각도가 0~360도가 아닐 때 0~360으로 조정해주는 함수. 
	double AdjustRotation(float& rot);

	//void clearDotweenTimerMap();

	CLONE(CDotween);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	Vec3 randPos;
	Vec3 randRange;

	TimerIndex currentTimerIndex;

	std::vector<Ptr<CDotweenTimer>> dotweenTimerVector;

	double previousAngle;

	float currentRotation{ 0 };


};

