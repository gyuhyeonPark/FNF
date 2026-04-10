#include "pch.h"
#include "CPlayerScript.h"

#include "RenderManager.h"

#include "CTransform.h"
#include "GameObject.h"
#include "CMissileScript.h"

#include "AssetManager.h"
#include "SceneManager.h"
#include "TaskManager.h"

CPlayerScript::CPlayerScript()
	: CScript(SCRIPT_TYPE::PLAYERSCRIPT)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Tick()
{
	if (GetOwner()->IsDead())
		int a = 0;
	Move();

	Shoot();

	Ptr<GameObject> pChild = GetOwner()->GetChild(0);

	Vec3 vRelativePos = pChild->Transform()->GetRelativePosition();
	Vec3 vWorldPos = pChild->Transform()->GetWorldPos();


	//Ptr<CCamera> pCam = RenderMgr::GetInstance()->GetPOVCamera();
	//Vec3 vCamPos = pCam->Transform()->GetPos();
	//Vec3 vMyPos = Transform()->GetPos();
	//Vec3 vDirToCam = vCamPos - vMyPos;
	//vDirToCam.Normalize();

	//Vec3 vBot = Vec3(vDirToCam.x, 0.f, vDirToCam.z).Normalize();
	//Vec3 vFront = Vec3(0.f, 0.f, -1.f);
	//	
	//float YDot = vBot.Dot(vFront);
	//float YAngle = acosf(YDot);

	//if (vFront.Cross(vBot).y < 0.f)
	//	YAngle = XM_2PI - YAngle;
	//	
	//float XDot = vBot.Dot(vDirToCam);
	//if (XDot > 1.f) XDot = 1.f;
	//if (XDot < -1.f) XDot = -1.f;

	//float XAngle = acosf(XDot);
	//if(vCamPos.y < vMyPos.y)
	//	XAngle = XM_2PI - XAngle;

	//Transform()->SetRotation(Vec3(0.f, YAngle, 0.f));
	//Transform()->SetRotation(Vec3(XAngle, 0.f, 0.f));
	//Transform()->SetRotation(Vec3(XAngle, YAngle, 0.f));
}

void CPlayerScript::Move()
{
	Vec3 vPos = Transform()->GetRelativePosition();
	Vec3 vScale = Transform()->GetRelativeScale();
	Vec3 vRotation = Transform()->GetRelativeRotation();

	Vec3 vUp = Transform()->GetDir(DIR::UP);
	Vec3 vDown = -vUp;

	if (KEY_PRESSED(KEY::UP))
		vPos += vUp * 100.f * DT;
	if (KEY_PRESSED(KEY::DOWN))
		vPos += vDown * 100.f * DT;

	if (KEY_PRESSED(KEY::RIGHT))
		vRotation.z -= XM_PI * DT;
	if (KEY_PRESSED(KEY::LEFT))
		vRotation.z += XM_PI * DT;

	Transform()->SetRelativePosition(vPos);
	Transform()->SetRelativeScale(vScale);
	Transform()->SetRelativeRotation(vRotation);
}

void CPlayerScript::Shoot()
{
	if (KEY_TAP(KEY::SPACE))
	{
		// 미사일 역할의 오브젝트 생성
		GameObject* pObject = new GameObject;
		pObject->SetName(L"Missile");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRenderer);
		//pObject->AddComponent(new CMissileScript());
		pObject->AddComponent(new CCollider2D);

		Vec3 vMyPos = Transform()->GetRelativePosition();
		Vec3 vMyScale = Transform()->GetRelativeScale();
		Vec3 vRotation = Transform()->GetRelativeRotation();
		Vec3 vUp = Transform()->GetDir(DIR::UP);

		pObject->Transform()->SetRelativePosition(vMyPos + vMyScale * 0.5f * vUp);
		pObject->Transform()->SetRelativeScale(Vec3(10.f, 30.f, 1.f));
		pObject->Transform()->SetRelativeRotation(vRotation);

		pObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
		pObject->MeshRenderer()->SetMaterial(AssetManager::GetInstance()->Find<AMaterial>(L"Std2DMtrl"));
		
		// 런타임 도중 GameObject를 생성할 땐 해당 함수를 호출한다. -> 다음 프레임에 적용 보장
		CreateObject(pObject, 1);
	}

	if (KEY_PRESSED(KEY::SPACE))
	{
	/*	DrawDebugRect(Transform()->GetRelativePosition()
			, Transform()->GetRelativeScale(), Vec3(0.f, 0.f, 0.f), Vec4(1.f, 1.f, 0.f, 1.f), 1.f);*/

		//DrawDebugCircle(GetOwner()->GetChild(0)->Transform()->GetWorldPos(), 100.f, Vec4(1.f, 0.f, 0.f, 1.f), 2.f);
		//GetOwner()->Destroy();
		//DrawDebugRect(Transform()->GetWorldMat(), Vec4(0.f, 1.f, 0.f, 1.f), 2.f);
	}	
}

