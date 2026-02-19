#include "pch.h"
#include "RenderManager.h"
#include "AssetManager.h"

RenderManager::RenderManager()
	: m_debugOption(false)
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::Init()
{
	m_dbgObj = new GameObject();
	m_dbgObj->AddComponent(new CTransform);
	m_dbgObj->AddComponent(new CMeshRenderer);
	m_dbgObj->GetMeshRenderer()->SetMtrl(FIND(AMaterial, L"DbgMtrl"));

	m_light2DBuffer = new StructuredBuffer;
}

void RenderManager::Progress()
{
	Graphics::GetInstance()->ClearView();
	
	if (KEY_TAP(KEY::F1))
		m_debugOption = !m_debugOption;
	// 렌더링 시작 전 업무
	Render_Start();

	// 카메라 기반 렌더링
	if (m_mainCamera == nullptr)
		return;

	m_mainCamera->Render();

	if (m_debugOption)
		Render_Debug();	

	Render_End();
}

void RenderManager::Render_Debug()
{
	list<DbgInfo>::iterator iter = m_dbgInfoList.begin();

	for (; iter != m_dbgInfoList.end();)
	{
		// Mesh 설정
		switch ((*iter).shape)
		{
		case DBG_SHAPE::RECT:
			m_dbgObj->GetMeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh_LineStrip"));
			break;
		case DBG_SHAPE::CIRCLE:
			m_dbgObj->GetMeshRenderer()->SetMesh(FIND(AMesh, L"CircleMesh_LineStrip"));
			break;
		case DBG_SHAPE::CUBE:
			m_dbgObj->GetMeshRenderer()->SetMesh(FIND(AMesh, L"CubeMesh"));
			break;
		case DBG_SHAPE::SPHERE:
			m_dbgObj->GetMeshRenderer()->SetMesh(FIND(AMesh, L"SphereMesh"));
			break;
		}

		// Transform 설정

		if ((*iter).matWorld == XMMatrixIdentity())
		{
			m_dbgObj->GetTransform()->SetRelativePosition((*iter).pos);
			m_dbgObj->GetTransform()->SetRelativeScale((*iter).scale);
			m_dbgObj->GetTransform()->SetRelativeRotation((*iter).rotation);
			m_dbgObj->Transform()->FinalTick();		// matrix 갱신
		}
		else
			m_dbgObj->Transform()->SetWorldMat((*iter).matWorld);

		// Material 설정
		m_dbgObj->MeshRenderer()->GetMtrl()->SetScalar(VEC4_0, (*iter).color);

		if ((*iter).depthTest)
			m_dbgObj->MeshRenderer()->GetMtrl()->GetShader()->SetDSType(DS_TYPE::LESS);
		else
			m_dbgObj->MeshRenderer()->GetMtrl()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);


		// Render 요청
		m_dbgObj->Render();

		(*iter).elapsed += DT;

		if ((*iter).duration <= (*iter).elapsed)
			iter = m_dbgInfoList.erase(iter);
		else
			++iter;
	}
}

void RenderManager::Render_Start()
{
	Graphics::GetInstance()->OMSetRTV();

	// 1. 등록받은 Light2D 의 광원 정보를 구조화버퍼에 담는다.
	// 2. 구조화 버퍼를 특정 t 레지스터에 바인딩한다.
	vector<Light2DInfo> vecInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecInfo.push_back(m_vecLight2D[i]->GetInfo());
	}

	if (!vecInfo.empty())
	{
		// 구조화 버퍼 공간이 모자라면 재확장 및 데이터 전달
		if (m_light2DBuffer->GetElementCount() < vecInfo.size())
			m_light2DBuffer->Create(sizeof(Light2DInfo), vecInfo.size(), SB_TYPE::SRV_ONLY, true, vecInfo.data());
		// 공간 여유가 있다면 바로 광원데이터 전달
		else
			m_light2DBuffer->SetData(vecInfo.data(), sizeof(Light2DInfo) * vecInfo.size());
	}

	// t12 레지스터 바인딩
	m_light2DBuffer->Binding(12);

	g_Global.Light2DCount = m_vecLight2D.size();

	// Globa 데이터를 상수버퍼를 통해서 b2 레지스터에 바인딩
	Graphics::GetInstance()->GetConstantBuffer(CB_TYPE::GLOBAL)->SetData(&g_Global);
	Graphics::GetInstance()->GetConstantBuffer(CB_TYPE::GLOBAL)->Binding();
}

void RenderManager::Render_End()
{
	// 구조화 버퍼 클리어
	// 등록받았던 광원들 해제
	m_light2DBuffer->Clear();
	m_vecLight2D.clear();
}
