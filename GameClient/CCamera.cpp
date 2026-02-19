#include "pch.h"
#include "GameObject.h"
#include "RenderManager.h"
#include "SceneManager.h"

CCamera::CCamera()
	:Component(COMPONENT_TYPE::CAMERA)
{
}

CCamera::~CCamera()
{
}

void CCamera::Begin()
{
	RenderManager::GetInstance()->RegisterCamera(this);
}

void CCamera::FinalTick()
{
	// 뷰행렬 계산
	Vec3 vPos = GetOwner()->GetTransform()->GetRelativePosition();

	// 이동 (카메라 위치를 원점으로 되돌리는 만큼의 이동행렬
	Matrix matTrans = XMMatrixIdentity();
	matTrans._41 = -vPos.x;
	matTrans._42 = -vPos.y;
	matTrans._43 = -vPos.z;
	
	Matrix matRot = XMMatrixIdentity();

	Vec3 right = GetOwner()->GetTransform()->GetDir(DIR::RIGHT);
	Vec3 front = GetOwner()->GetTransform()->GetDir(DIR::FRONT);
	Vec3 up = GetOwner()->GetTransform()->GetDir(DIR::UP);

	matRot._11 = right.x;	matRot._12 = up.x;	matRot._13 = front.x;
	matRot._21 = right.y;	matRot._22 = up.y;	matRot._23 = front.y;
	matRot._31 = right.z;	matRot._32 = up.z;	matRot._33 = front.z;

	// view 행렬 회전 - 카메라가 바라보는 방향을 z축이 되도록 회전하기
	m_MatView = matTrans * matRot;



	// 회전


	// 투영(Projection) 행렬 계산
	// Orthographic
	if (m_ProjType == PROJ_TYPE::ORTHOGRAPHIC)
		m_MatProj = XMMatrixOrthographicLH(m_Width, m_Width / m_AspectRatio, 1.f, m_Far);

	// 원근 투영 (Perspective)
	else if (m_ProjType == PROJ_TYPE::PERSPECTIVE)
		m_MatProj = XMMatrixPerspectiveFovLH(m_Fov, m_AspectRatio, 1.f, m_Far);
}

void CCamera::LayerCheck(int idx)
{
	m_layerCheck ^= (1 << idx);		// XOR 연산 -> 켜져있다면 꺼주고, 꺼져있으면 켜준다
}

void CCamera::Render()
{
	g_Trans.matView = m_MatView;
	g_Trans.matProj = m_MatProj;

	Ptr<AScene> curScene = SceneManager::GetInstance()->GetCurrentScene();

	for (UINT i = 0; i < MAX_LAYER; i++)
	{
		if ((m_layerCheck & (1 << i)) == false)
			continue;

		Layer* layer = curScene->GetLayer(i);
		layer->Render();
	}
}
