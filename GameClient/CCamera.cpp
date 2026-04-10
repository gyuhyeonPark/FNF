#include "pch.h"
#include "GameObject.h"
#include "RenderManager.h"
#include "SceneManager.h"

CCamera::CCamera()
	:Component(COMPONENT_TYPE::CAMERA), m_OrthoScale(1.f)
{
}

CCamera::~CCamera()
{
}

void CCamera::Begin()
{
	RenderManager::GetInstance()->RegisterCamera(this, m_camType);
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




	float scaledWidth = m_Width / m_OrthoScale;
	float scaledHeight = scaledWidth / m_AspectRatio;

	// 투영(Projection) 행렬 계산
	// Orthographic
	if (m_ProjType == PROJ_TYPE::ORTHOGRAPHIC)
		m_MatProj = XMMatrixOrthographicLH(scaledWidth, scaledHeight, 1.f, m_Far);

	// 원근 투영 (Perspective)
	else if (m_ProjType == PROJ_TYPE::PERSPECTIVE)
		m_MatProj = XMMatrixPerspectiveFovLH(m_Fov, m_AspectRatio, 1.f, m_Far);
}

void CCamera::LayerCheck(int idx)
{
	m_layerCheck ^= (1 << idx);		// XOR 연산 -> 켜져있다면 꺼주고, 꺼져있으면 켜준다
}

void CCamera::SortObject()
{
	// 렌더링 할 물체들을 정렬한다.
	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTrapsnarent.clear();
	m_vecPostProcess.clear();

	Ptr<AScene> pCurLevel = SceneManager::GetInstance()->GetCurrentScene();
	if (pCurLevel == nullptr)
		return;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 카메라가 레이어를 볼 수 있어야 함
		if (false == (m_layerCheck & (1 << i)))
			continue;

		// 레이어에 소속된 모든 오브젝트를 가져온다
		Layer* pLayer = pCurLevel->GetLayer(i);
		const vector<Ptr<GameObject>>& vecObjects = pLayer->GetAllObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// 오브젝트가 렌더링을 할 수 있는 상태인지 확인
			if (nullptr == vecObjects[j]->GetRenderer()
				|| nullptr == vecObjects[j]->GetRenderer()->GetMesh()
				|| nullptr == vecObjects[j]->GetRenderer()->GetMaterial())
			{
				continue;
			}

			RENDER_DOMAIN domain = vecObjects[j]->GetRenderer()->GetMaterial()->GetDomain();

			switch (domain)
			{
			case RENDER_DOMAIN::DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j].Get());
				break;
			case RENDER_DOMAIN::DOMAIN_MASKED:
				m_vecMasked.push_back(vecObjects[j].Get());
				break;
			case RENDER_DOMAIN::DOMAIN_TRANSPARENT:
				m_vecTrapsnarent.push_back(vecObjects[j].Get());
				break;
			case RENDER_DOMAIN::DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j].Get());
				break;

			}
		}
	}
}

void CCamera::Render()
{
	g_Trans.matView = m_MatView;
	g_Trans.matProj = m_MatProj;

	// Domain 순서대로 렌더링 진행
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
		m_vecOpaque[i]->Render();

	for (size_t i = 0; i < m_vecMasked.size(); ++i)
		m_vecMasked[i]->Render();

	for (size_t i = 0; i < m_vecTrapsnarent.size(); ++i)
		m_vecTrapsnarent[i]->Render();

	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
		m_vecPostProcess[i]->Render();
}

void CCamera::SaveToSceneFile(FILE* _File)
{
	fwrite(&m_layerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_Fov, sizeof(float), 1, _File);
	fwrite(&m_OrthoScale, sizeof(float), 1, _File);
}

void CCamera::LoadFromSceneFile(FILE* _File)
{
	fread(&m_layerCheck, sizeof(UINT), 1, _File);
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_Fov, sizeof(float), 1, _File);
	fread(&m_OrthoScale, sizeof(float), 1, _File);
}
