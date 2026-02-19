#include "pch.h"
#include "Dotween.h"
#include "CTransform.h"
#include "DotweenTimer.h"
//#include <cmath>
//#include <float.h>
//#include "DotweenTimerPool.h"
//#include "ContentsLayer.h"
//#include "Application.h"

Dotween::Dotween()
{
	for (int i = 0; i < UINT(UINT(TimerIndex::End)); i++)
	{
		dotweenTimerVector.push_back(new DotweenTimer);
	}
}

Dotween::~Dotween()
{
}

void Dotween::Tick()
{
	for (int i = 0; i < UINT(UINT(TimerIndex::End)); i++)
	{
		if (dotweenTimerVector[i] != nullptr)
		{
			dotweenTimerVector[i]->Update();
		}
	}
}

void Dotween::StopAllDotweenFunction()
{
	for (int i = 0; i < UINT(TimerIndex::End); i++)
	{
		if (dotweenTimerVector[i] != nullptr)
		{
			/// isDone을 true로 바꿔주어 강제로 타이머 종료.
			dotweenTimerVector[i]->isDone = true;
			dotweenTimerVector[i]->isActive = false;
		}
	}
}

Ptr<Dotween> Dotween::DOMove(Vec3 endPosition, double p_duration)
{
	// 	GetOwner()->GetTransform()는 Component를 갖게 될 GameObject를 리턴한다.
	DotweenTimer* m_doMovetweenTimer = dotweenTimerVector[UINT(TimerIndex::MoveTimer)].Get();
	m_doMovetweenTimer->Init();

	// 시작 값 저장
	m_doMovetweenTimer->m_xValue = GetOwner()->GetTransform()->GetRelativePosition().x;
	m_doMovetweenTimer->m_yValue = GetOwner()->GetTransform()->GetRelativePosition().y;
	m_doMovetweenTimer->m_zValue = GetOwner()->GetTransform()->GetRelativePosition().z;

	// 가야 하는 거리
	double distanceX = endPosition.x - GetOwner()->GetTransform()->GetRelativePosition().x;
	double distanceY = endPosition.y - GetOwner()->GetTransform()->GetRelativePosition().y;
	double distanceZ = endPosition.z - GetOwner()->GetTransform()->GetRelativePosition().z;

	m_doMovetweenTimer->Start();
	m_doMovetweenTimer->duration = p_duration;
	m_doMovetweenTimer->onUpdate = [=]()
		{
			// 시간 정규화 : 진행 시간 / 전체 기간
			m_doMovetweenTimer->easeTime = (m_doMovetweenTimer->elapsed - m_doMovetweenTimer->delay) / (m_doMovetweenTimer->duration - m_doMovetweenTimer->delay);

			// 가야하는 거리의 정규화...
			// 진행 거리 / 전체 가야하는 거리
			// 진행 거리 = 시간 * 속력

			double easeWeight = getEasingFunction(m_doMovetweenTimer->m_ease)(m_doMovetweenTimer->easeTime);

			m_doMovetweenTimer->movingDistanceX = easeWeight * (distanceX);
			m_doMovetweenTimer->movingDistanceY = easeWeight * (distanceY);
			m_doMovetweenTimer->movingDistanceZ = easeWeight * (distanceZ);

			m_doMovetweenTimer->finalXvalue = m_doMovetweenTimer->m_xValue + m_doMovetweenTimer->movingDistanceX;
			m_doMovetweenTimer->finalYvalue = m_doMovetweenTimer->m_yValue + m_doMovetweenTimer->movingDistanceY;
			m_doMovetweenTimer->finalZvalue = m_doMovetweenTimer->m_zValue + m_doMovetweenTimer->movingDistanceZ;

			if (m_doMovetweenTimer->easeTime >= 1)
			{
				m_doMovetweenTimer->finalXvalue = endPosition.x;
				m_doMovetweenTimer->finalYvalue = endPosition.y;
				m_doMovetweenTimer->finalZvalue = endPosition.z;
			}

			GetOwner()->GetTransform()->SetRelativePosition(Vec3(m_doMovetweenTimer->finalXvalue, m_doMovetweenTimer->finalYvalue, m_doMovetweenTimer->finalZvalue));
		};

	currentTimerIndex = TimerIndex::MoveTimer;

	/// 자기 자신을 반환해준다...뒤에 SetDelay, SetEase 등을 위해
	return this;
}

// DOScale, DoRotate 등의 함수도 제작 예정...
// 결국 Vector3의 값을 변경해주는 거라 금방 만들 수 있다
Ptr<Dotween> Dotween::DOScale(Vec3 endScale, double p_duration)
{
	DotweenTimer* m_doScaletweenTimer = dotweenTimerVector[UINT(TimerIndex::ScaleTimer)].Get();
	m_doScaletweenTimer->Init();

	m_doScaletweenTimer->m_xValue = GetOwner()->GetTransform()->GetRelativeScale().x;
	m_doScaletweenTimer->m_yValue = GetOwner()->GetTransform()->GetRelativeScale().y;
	m_doScaletweenTimer->m_zValue = GetOwner()->GetTransform()->GetRelativeScale().z;

	double distanceX = endScale.x - GetOwner()->GetTransform()->GetRelativeScale().x;
	double distanceY = endScale.y - GetOwner()->GetTransform()->GetRelativeScale().y;
	double distanceZ = endScale.z - GetOwner()->GetTransform()->GetRelativeScale().z;

	m_doScaletweenTimer->Start();
	m_doScaletweenTimer->duration = p_duration;
	m_doScaletweenTimer->onUpdate = [=]()
		{
			// 소수끼리의 나눗셈 연산은 정확한 값을 만들어 줘야 한다...
			m_doScaletweenTimer->easeTime = (m_doScaletweenTimer->elapsed - m_doScaletweenTimer->delay) / (m_doScaletweenTimer->duration - m_doScaletweenTimer->delay);

			double easeWeight = getEasingFunction(m_doScaletweenTimer->m_ease)(m_doScaletweenTimer->easeTime);

			// 아래 세줄 안에 문제가 없다면 내 손에 장을 지짐 - 이윤우
			m_doScaletweenTimer->movingDistanceX = easeWeight * (distanceX);
			m_doScaletweenTimer->movingDistanceY = easeWeight * (distanceY);
			m_doScaletweenTimer->movingDistanceZ = easeWeight * (distanceZ);

			m_doScaletweenTimer->finalXvalue = m_doScaletweenTimer->m_xValue + m_doScaletweenTimer->movingDistanceX;
			m_doScaletweenTimer->finalYvalue = m_doScaletweenTimer->m_yValue + m_doScaletweenTimer->movingDistanceY;
			m_doScaletweenTimer->finalZvalue = m_doScaletweenTimer->m_zValue + m_doScaletweenTimer->movingDistanceZ;

			/// 부동 소수점 나눗셈 연산이 문제인듯... 값이 커질수록 버리는 수가 많아 진다....

			if (m_doScaletweenTimer->easeTime >= 1)
			{
				m_doScaletweenTimer->finalXvalue = endScale.x;
				m_doScaletweenTimer->finalYvalue = endScale.y;
				m_doScaletweenTimer->finalZvalue = endScale.z;
			}

			GetOwner()->GetTransform()->SetRelativeScale(Vec3(m_doScaletweenTimer->finalXvalue, m_doScaletweenTimer->finalYvalue, m_doScaletweenTimer->finalZvalue));
		};

	currentTimerIndex = TimerIndex::ScaleTimer;

	return this;
}


Ptr<Dotween> Dotween::DORotate(Vec3 endRotation, double p_duration)
{
	/// DORotate 만들 때 주의할 점.
	/// 1. 각이 360을 넘어가면 0으로 초기화 해준다. 
	/// 일단 여기까지?
	DotweenTimer* m_doRotatetweenTimer = dotweenTimerVector[UINT(TimerIndex::RotateTimer)].Get();
	m_doRotatetweenTimer->Init();

	m_doRotatetweenTimer->m_xValue = GetOwner()->GetTransform()->GetRelativeRotation().x;
	m_doRotatetweenTimer->m_yValue = GetOwner()->GetTransform()->GetRelativeRotation().y;
	m_doRotatetweenTimer->m_zValue = GetOwner()->GetTransform()->GetRelativeRotation().z;

	double distanceX = endRotation.x - GetOwner()->GetTransform()->GetRelativeRotation().x;
	double distanceY = endRotation.y - GetOwner()->GetTransform()->GetRelativeRotation().y;
	double distanceZ = endRotation.z - GetOwner()->GetTransform()->GetRelativeRotation().z;

	m_doRotatetweenTimer->Start();
	m_doRotatetweenTimer->duration = p_duration;
	m_doRotatetweenTimer->onUpdate = [=]()
		{
			m_doRotatetweenTimer->easeTime = (m_doRotatetweenTimer->elapsed - m_doRotatetweenTimer->delay) / (m_doRotatetweenTimer->duration - m_doRotatetweenTimer->delay);

			double easeWeight = getEasingFunction(m_doRotatetweenTimer->m_ease)(m_doRotatetweenTimer->easeTime);

			m_doRotatetweenTimer->movingDistanceX = easeWeight * (distanceX);
			m_doRotatetweenTimer->movingDistanceY = easeWeight * (distanceY);
			m_doRotatetweenTimer->movingDistanceZ = easeWeight * (distanceZ);

			m_doRotatetweenTimer->finalXvalue = m_doRotatetweenTimer->m_xValue + m_doRotatetweenTimer->movingDistanceX;
			m_doRotatetweenTimer->finalYvalue = m_doRotatetweenTimer->m_yValue + m_doRotatetweenTimer->movingDistanceY;
			m_doRotatetweenTimer->finalZvalue = m_doRotatetweenTimer->m_zValue + m_doRotatetweenTimer->movingDistanceZ;


			if (m_doRotatetweenTimer->easeTime >= 1)
			{
				m_doRotatetweenTimer->finalXvalue = endRotation.x;
				m_doRotatetweenTimer->finalYvalue = endRotation.y;
				m_doRotatetweenTimer->finalZvalue = endRotation.z;

				AdjustRotation(m_doRotatetweenTimer->finalXvalue);
				AdjustRotation(m_doRotatetweenTimer->finalYvalue);
				AdjustRotation(m_doRotatetweenTimer->finalZvalue);
			}

			// 각이 360을 넘어가면 0으로 초기화 해준다.
			// SetRelativeRotation 직전에 바꿔주기
			if (m_doRotatetweenTimer->finalXvalue >= 360)
				AdjustRotation(m_doRotatetweenTimer->finalXvalue);

			if (m_doRotatetweenTimer->finalYvalue >= 360)
				AdjustRotation(m_doRotatetweenTimer->finalYvalue);

			if (m_doRotatetweenTimer->finalZvalue >= 360)
				AdjustRotation(m_doRotatetweenTimer->finalZvalue);

			/// 반대로 각도가 -360 미만이라면 -360 ~ 0으로 바꿔준다.
			if (m_doRotatetweenTimer->finalXvalue < 0)
				AdjustRotation(m_doRotatetweenTimer->finalXvalue);

			if (m_doRotatetweenTimer->finalYvalue < 0)
				AdjustRotation(m_doRotatetweenTimer->finalYvalue);

			if (m_doRotatetweenTimer->finalZvalue < 0)
				AdjustRotation(m_doRotatetweenTimer->finalZvalue);

			GetOwner()->GetTransform()->SetRelativeRotation(Vec3(m_doRotatetweenTimer->finalXvalue, m_doRotatetweenTimer->finalYvalue, m_doRotatetweenTimer->finalZvalue));
		};

	currentTimerIndex = TimerIndex::RotateTimer;

	return this;
}

Ptr<Dotween> Dotween::DOQRotate(Vec3 axis, double angle, double p_duration)
{
	DotweenTimer* m_doQrotatetweenTimer = dotweenTimerVector[UINT(TimerIndex::RotateTimer)].Get();
	m_doQrotatetweenTimer->Init();

	m_doQrotatetweenTimer->Start();
	m_doQrotatetweenTimer->duration = p_duration;
	m_doQrotatetweenTimer->onUpdate = [=]()
		{
			// 결국 돌려야 하는건 angle, 그런데 ease를 적용한...
			m_doQrotatetweenTimer->easeTime = (m_doQrotatetweenTimer->elapsed - m_doQrotatetweenTimer->delay) / (m_doQrotatetweenTimer->duration - m_doQrotatetweenTimer->delay);
			double easeWeight = getEasingFunction(m_doQrotatetweenTimer->m_ease)(m_doQrotatetweenTimer->easeTime);

			//double anglePerFrame = angle / (p_duration);

			previousAngle = m_doQrotatetweenTimer->movingDistanceX;

			m_doQrotatetweenTimer->movingDistanceX = easeWeight * angle;

			//GetOwner()->GetTransform()->Rotate(axis, DegreeToRadian(m_doQrotatetweenTimer->movingDistanceX - previousAngle));
		};

	currentTimerIndex = TimerIndex::RotateTimer;

	return this;
}

/// <summary>
/// DOLookAt 함수는 y축을 고려하지 않았다. (위 아래로 회전하지 않는다.)
/// </summary>
/// <param name="lookPosition"></param>
/// <returns></returns>
Ptr<Dotween> Dotween::DOLookAt(Vec3 lookTransform, double p_duration, bool isYaxisInclude)
{
	//StopAllDotweenFunction();
	//DotweenTimer* m_doLookTweenTimer = dotweenTimerVector[TimerIndex::RotateTimer];
	//m_doLookTweenTimer->Init();

	//Vec3 objectFront = GetOwner()->GetTransform()->GetRelativeRotation().Forward();
	//objectFront *= -1;		// 모델링 뒤집힌 걸 감안
	//Vec3 objectUp;
	//Vec3 objectRight = GetOwner()->GetTransform()->GetRelativeRotation().Right();
	//Vec3 objectLeft = Vec3(-1 * objectRight.x, objectRight.y, -1 * objectRight.z);

	//Vec3 tempLookPos = lookTransform;
	//Vec3 tempPos = GetOwner()->GetTransform()->GetRelativePosition();
	//Vec3 tempUp = GetOwner()->GetTransform()->GetRelativeRotation().Up();

	//Vec3 distanceVec = Vec3(tempLookPos.x - tempPos.x, tempLookPos.y - tempPos.y, tempLookPos.z - tempPos.z);

	//double angle;
	//double sq;
	//double finalAngle;
	//double finalDegree;

	//double dot = Vec3::Dot(GetOwner()->GetTransform()->GetRelativeRotation().Forward(), GetOwner()->GetTransform()->GetRelativePosition() - tempLookPos);

	//// 회전 방향 판정
	//Vec3 axis = Vec3::Cross(objectFront, distanceVec);

	//angle = (objectFront.x * distanceVec.x + objectFront.z * distanceVec.z);
	//sq = (sqrt(pow(objectFront.x, 2) + pow(objectFront.z, 2)) *
	//	sqrt(pow(distanceVec.x, 2) + pow(distanceVec.z, 2)));

	//// 두 벡터의 각도가 180도 이상이면 180을, -180 이하 이라면 -180을 
	////finalAngle = acos( max( -1.0f, min(1.0f, angle / sq) ) );
	//finalAngle = acos(std::clamp(angle / sq, -1.0, 1.0));			// c++17 된다면
	//finalDegree = 57.2969f * (finalAngle);

	//if (axis.y < 0)
	//	finalDegree *= -1;

	//m_doLookTweenTimer->Start();
	//m_doLookTweenTimer->duration = p_duration;
	//m_doLookTweenTimer->onUpdate = [=]()
	//	{
	//		double degreePerFrame = finalDegree / (m_doLookTweenTimer->duration);
	//		currentRotation = degreePerFrame * Time::GetDeltaTime();
	//		//assert(!isnan(currentRotation));
	//		if (!isnan(currentRotation))
	//			GetOwner()->GetTransform()->SetRelativeRotation(GetOwner()->GetTransform()->GetRelativeRotation() * Quaternion({ 0.0f, currentRotation, 0.0f }));
	//	};

	//currentTimerIndex = TimerIndex::RotateTimer;

	return this;
}


/// <summary>
/// dotweenTimer의 OnUpdate를 직접 설정해주고 실행가능한 함수.
/// 그럴려면 꼭! OnUpdate를 넣어줘야한다!
/// </summary>
/// <param name="p_duration"></param>
/// <returns></returns>
Ptr<Dotween> Dotween::DOCustom(double p_duration)
{
	DotweenTimer* m_doCustomTweenTimer = dotweenTimerVector[UINT(TimerIndex::CustomTimer)].Get();
	m_doCustomTweenTimer->Init();

	m_doCustomTweenTimer->Start();
	m_doCustomTweenTimer->duration = p_duration;

	currentTimerIndex = TimerIndex::CustomTimer;

	return this;
}

Ptr<Dotween> Dotween::DONothing(double p_duration)
{
	DotweenTimer* m_doNothingTweenTimer = dotweenTimerVector[UINT(TimerIndex::NothingTimer)].Get();

	m_doNothingTweenTimer->duration = p_duration;

	currentTimerIndex = TimerIndex::NothingTimer;

	return this;

}

Ptr<Dotween> Dotween::DOShakePosition(double p_duration, double strength /*= 1.5f*/, int vibrato /*= 1.5*/)
{
	DotweenTimer* m_doShaketweenTimer = dotweenTimerVector[UINT(TimerIndex::ShakeTimer)].Get();
	m_doShaketweenTimer->Init();

	m_doShaketweenTimer->Start();
	m_doShaketweenTimer->duration = p_duration;
	randPos = Vec3(0.f, 0.f, 0.f);

	m_doShaketweenTimer->onUpdate = [=]()
		{
			GetOwner()->GetTransform()->SetRelativePosition(GetOwner()->GetTransform()->GetRelativePosition() - randPos);

			int plusORminus = rand() % 2;

			randRange = Vec3(rand() % vibrato, rand() % vibrato, 0.0f);

			randRange *= strength;

			switch (plusORminus)
			{
			case 0:
				randPos = randRange;
				break;

			case 1:
				randPos = Vec3(randRange.x * -1, randRange.y * -1, 0);
				break;
			}

			GetOwner()->GetTransform()->SetRelativePosition(GetOwner()->GetTransform()->GetRelativePosition() + randPos);
		};

	m_doShaketweenTimer->onExpiration = [=]()
		{
			GetOwner()->GetTransform()->SetRelativePosition(GetOwner()->GetTransform()->GetRelativePosition() - randPos);
		};

	currentTimerIndex = TimerIndex::ShakeTimer;

	return this;
}

Ptr<Dotween> Dotween::SetDelay(double p_delay)
{
	dotweenTimerVector[UINT(currentTimerIndex)]->delay = p_delay;
	dotweenTimerVector[UINT(currentTimerIndex)]->duration += p_delay;

	return this;
}

Ptr<Dotween> Dotween::IsRepeat(bool p_repeat)
{
	dotweenTimerVector[UINT(currentTimerIndex)]->isRepeat = p_repeat;
	return this;
}

Ptr<Dotween> Dotween::SetEase(easing_functions p_ease)
{
	dotweenTimerVector[UINT(currentTimerIndex)]->m_ease = p_ease;

	return this;
}

Ptr<Dotween> Dotween::OnComplete(std::function<void()> expirationFunc)
{
	dotweenTimerVector[UINT(currentTimerIndex)]->onCompleteFunction = expirationFunc;
	return this;
}

Ptr<Dotween> Dotween::OnUpdate(std::function<void()> updateFunc)
{
	dotweenTimerVector[UINT(currentTimerIndex)]->onUpdate = updateFunc;
	return this;
}

double Dotween::AdjustRotation(float& rot)
{
	while (rot > 360 || rot < 0)
	{
		if (rot < 0)
			rot += 360;
		else if (rot > 360)
			rot -= 360;
	}
	return rot;
}

//void Dotween::clearDotweenTimerMap()
//{
//	if (tempTimer != nullptr)
//	{
//		tempTimer->onCompleteFunction = []() {};
//		tempTimer->onExpiration = []() {};
//		tempTimer->onUpdate = []() {};
//	}
//
//	auto it = m_dotweenTimerMap.begin();
//	while (it != m_dotweenTimerMap.end())
//	{
//		it->first->onUpdate = []() {};
//		it->first->onExpiration = []() {};
//		it->first->onCompleteFunction = []() {};
//		it++;
//	}
//}
