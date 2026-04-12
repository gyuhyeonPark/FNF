#include "pch.h"
#include "func.h"
#include "RenderManager.h"
#include "TaskManager.h"
#include "GameObject.h"

void CreateObject(GameObject* object, int layeridx)
{
	TaskInfo info = {};

	info.type = TASK_TYPE::CREATE_OBJECT;
	info.param_0 = (DWORD_PTR)object;
	info.param_1 = layeridx;

	TaskManager::GetInstance()->AddTask(info);
}

wchar_t Buff[255] = {};
void ChangeScene(const wstring& _NextSceneName)
{
	TaskInfo info = {};

	wcscpy_s(Buff, 255, _NextSceneName.c_str());

	info.type = TASK_TYPE::CHANGE_SCENE;
	info.param_0 = (DWORD_PTR)Buff;

	TaskManager::GetInstance()->AddTask(info);
}

void ChangeSceneState(SCENE_STATE _NextState)
{
	TaskInfo info = {};

	info.type = TASK_TYPE::CHANGE_SCENE_STATE;
	info.param_0 = (DWORD_PTR)_NextState;

	TaskManager::GetInstance()->AddTask(info);
}

void DrawDebugRect(Vec3 pos, Vec3 scale, Vec3 rot, Vec4 color, float duration, bool depthTest)
{
	DbgInfo info = {};

	info.shape = DBG_SHAPE::RECT;
	info.pos = pos;
	info.scale = scale;
	info.rotation = rot;
	info.matWorld = XMMatrixIdentity();
	info.color = color;
	info.elapsed = 0.f;
	info.duration = duration;
	info.depthTest = depthTest;
	RenderManager::GetInstance()->AddDebugInfo(info);
}

void DrawDebugRect(Matrix matWorld, Vec4 color, float duration, bool depthTest)
{
	DbgInfo info = {};

	info.shape = DBG_SHAPE::RECT;

	info.matWorld = matWorld;
	info.color = color;
	info.elapsed = 0.f;
	info.duration = duration;
	info.depthTest = depthTest;
	RenderManager::GetInstance()->AddDebugInfo(info);
}

void DrawDebugCircle(Vec3 pos, float radius, Vec4 color, float duration, bool depthTest)
{
	DbgInfo info = {};

	info.shape = DBG_SHAPE::CIRCLE;
	info.pos = pos;
	info.scale = Vec3(radius * 2.f, radius * 2.f, 0.f);
	info.rotation = Vec3(0.f, 0.f, 0.f);
	info.color = color;
	info.elapsed = 0.f;
	info.duration = duration;
	info.depthTest = depthTest;
	RenderManager::GetInstance()->AddDebugInfo(info);
}

bool IsValid(Ptr<GameObject>& obj)
{
	if (obj == nullptr || obj->IsDead())
	{
		obj = nullptr;
		return false;
	}

	return true;
}

void SaveWString(FILE* _File, const wstring& _String)
{
	int Len = _String.length();
	fwrite(&Len, sizeof(int), 1, _File);
	fwrite(_String.data(), sizeof(wchar_t), Len, _File);
}

void SaveAssetRef(FILE* _File, Asset* _Asset)
{
	// Asset 이 Null 인지 아닌지 저장
	bool IsNull = _Asset;
	fwrite(&IsNull, sizeof(bool), 1, _File);

	// Asset 의 Key, RelativePath 저장
	if (nullptr != _Asset)
	{
		SaveWString(_File, _Asset->GetKey());
		SaveWString(_File, _Asset->GetRelativePath());
	}
}

wstring LoadWString(FILE* _File)
{
	int Len = 0;
	fread(&Len, sizeof(int), 1, _File);

	wchar_t buff[255] = {};
	fread(buff, sizeof(wchar_t), Len, _File);

	return buff;
}

float Saturate(float _Data)
{
	if (1.f < _Data)
		return 1.f;
	else if (_Data < 0.f)
		return 0.f;
	else
		return _Data;
}










#include "AMesh.h"
#include "AGrapicShader.h"
#include "ATexture.h"
#include "GameObject.h"
#include "ConstantBuffer.h"
#include "Source\Scripts\CPlayerScript.h"
#include "Source\Scripts\COpponentController.h"
#include "AssetManager.h"
#include "Source\Scripts\CAtlasAnimator.h"
#include "CImage.h"
#include "Source\Scripts\CDotween.h"
#include "Source\Scripts\Easing.h"
#include "Source\Scripts\CGameManager.h"
#include "Source\Scripts\CPlayerController.h"
#include "Source\Scripts\CCameraMove.h"
#include "Source\Scripts\CStartingPistol.h"
#include "Source\Scripts\CEventManager.h"
#include "Source\Scripts\CPlayerNoteReceptor.h"
#include "Source\Scripts\COpponentNoteReceptor.h"
#include "Source\Scripts\CNote.h"
#include "BeatCamera.h"
#include "AssetKeys.h"
#include "SongManager.h"
#include "TitleScript.h"
#include "CStageTransition.h"
#include "CStageLoader.h"



void CreateSongEditorScene()
{
	SongManager::GetInstance()->ChangeSong(STAGENUM::BOPEEBO);

	// Scene 생성
	Ptr<AScene> pScene = new AScene;
	pScene->SetName(L"SongEditor");

	Ptr<GameObject> pObject = nullptr;

	// 카메라 역할 오브젝트 
	Ptr<GameObject> camObj = new GameObject;
	camObj->SetName(L"MainCamera");

	Ptr<CCamera> camComp = new CCamera;
	camComp->SetCamType(CamIdx::MAINCAM);
	camObj->AddComponent(new CTransform);
	camObj->AddComponent(camComp.Get());
	camObj->AddComponent(new CCameraMove);
	camObj->AddComponent(new CDotween);
	//camObj->AddComponent(new BeatCamera);

	camObj->Camera()->LayerCheckAll();
	camObj->Camera()->LayerCheck(31);

	camObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	camObj->Camera()->SetFar(10000.f);
	camObj->Camera()->SetFOV(90.f);
	camObj->Camera()->SetOrthoScale(1.3f);
	Vec2 vResolution = Vec2(GWinSizeX, GWinSizeY);
	camObj->Camera()->SetAspectRatio(vResolution.x / vResolution.y); // 종횡비(AspectRatio)
	camObj->Camera()->SetWidth(vResolution.x);

	camObj->Transform()->SetRelativePosition(Vec3(0.f, 0.f, -100.f));

	// HUD camera
	Ptr<GameObject> UICamObj = new GameObject;
	UICamObj->SetName(L"HUDCamera");
	UICamObj->AddComponent(new CTransform);
	UICamObj->AddComponent(new CCamera);
	UICamObj->AddComponent(new CCameraMove);
	UICamObj->Camera()->LayerCheckClear();
	UICamObj->Camera()->LayerCheck(31);

	UICamObj->Camera()->SetCamType(CamIdx::HUDCAM);
	UICamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	UICamObj->Camera()->SetFar(10000.f);
	UICamObj->Camera()->SetFOV(90.f);
	UICamObj->Camera()->SetOrthoScale(1.f);
	UICamObj->Camera()->SetAspectRatio(vResolution.x / vResolution.y); // 종횡비(AspectRatio)
	UICamObj->Camera()->SetWidth(vResolution.x);

	UICamObj->Transform()->SetRelativePosition(Vec3(0.f, 0.f, -100.f));

#pragma region FNF Contents

#pragma region Game Manager, CNote Receptors
	Ptr<CGameManager> gm = new CGameManager;
	gm->SetDifficulty(DIFF);
	gm->SetCamComp(camComp);
	Ptr<GameObject> gmObj = new GameObject();
	gmObj->SetName(L"GM");
	gmObj->AddComponent(gm.Get());
	gmObj->AddComponent(new CTransform);
	pScene->AddObject(0, gmObj);

	// 1. CNote Receptor 오브젝트 생성
	// 스프라이트 출력, 컴포넌트 세팅 등...

	// 각 NR들은 하나의 이미지를 공유한다.
	Ptr<ATexture> nrTexture = new ATexture();
	nrTexture = AssetManager::GetInstance()->Find<ATexture>(L"notereceptor");

	Vec3 opponentNRPos = Vec3(-400.f, 200.f, 20.f);
	Vec3 playerNRPos = Vec3(100.f, 200.f, 20.f);

	for (UINT i = 0; i < NOTE_SIZE; i++)
	{
		Ptr<CAtlasAnimator> animator = new CAtlasAnimator;
		animator->LoadMapInfo(nrTexture);
		Ptr<CNoteReceptor> nrCom;
		if (i < (UINT)ARROW_DIR::END)
			nrCom = new CPlayerNoteReceptor();
		else
			nrCom = new COpponentNoteReceptor();

		nrCom->SetAnimator(animator);

		gm->SetNoteReceptor(i, nrCom);

		Ptr<GameObject> nrObj = new GameObject();
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"NoteReceptor_%d", i);
		nrObj->SetName(Buff);
		nrObj->AddComponent(new CTransform);
		nrObj->AddComponent(new CMeshRenderer);
		nrObj->AddComponent(nrCom.Get());
		nrObj->AddComponent(animator.Get());

		nrObj->AddComponent(new CDotween);

		Ptr<AMesh> nrMesh = (AMesh*)AssetManager::GetInstance()->FindAsset(ASSET_TYPE::MESH, L"RectMesh").Get();
		Ptr<AMaterial> nrMaterial = AssetManager::GetInstance()->Find<AMaterial>(L"NoteReceptor");

		nrObj->GetMeshRenderer()->SetMesh(nrMesh);
		nrObj->GetMeshRenderer()->SetMaterial(nrMaterial);
		nrObj->GetTransform()->SetIndependentScale(true);
		nrObj->GetTransform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
		UICamObj->AddChild(nrObj);

		if (i < (UINT)ARROW_DIR::END)
		{
			nrObj->GetTransform()->SetRelativePosition(playerNRPos);
			playerNRPos.x += 100.f;
		}
		else
		{
			nrObj->GetTransform()->SetRelativePosition(opponentNRPos);
			opponentNRPos.x += 100.f;
		}

		nrCom->SetDir(ARROW_DIR(i % 4));
		nrCom->AtlasAnimator()->Play(defaultArrowName[i % 4], true);
	}

	// 곡에 대한 노트 정보를 각각의 Receptor에게 전달.
	// Editor에서는 노트 정보가 없으므로 X
	//PARSER->LoadNoteData(SONGNAME, gm->GetNoteReceptors());
#pragma endregion

#pragma region Note 생성 로직
	// 생성하지 아니한다.
#pragma endregion

	// Player Object 추가
	Ptr<GameObject> playerObject = new GameObject;
	playerObject->SetName(L"Player");
	playerObject->AddComponent(new CTransform);
	playerObject->AddComponent(new CMeshRenderer);
	//pObject->AddComponent(new CPlayerScript);

	Ptr<CAtlasAnimator> playerAnim = new CAtlasAnimator();
	playerAnim->LoadMapInfo(FIND(ATexture, L"bf"));
	playerObject->AddComponent(playerAnim.Get());

	Ptr<CPlayerController> pController = new CPlayerController();
	pController->SetAnimator(playerAnim);
	playerObject->AddComponent(pController.Get());
	gm->SetPlayerController(pController.Get());

	playerObject->AddComponent(new CCollider2D);
	playerObject->Transform()->SetRelativePosition(Vec3(216.f, -193.f, 1.f));
	playerObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	playerObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	playerObject->MeshRenderer()->SetMaterial(AssetManager::GetInstance()->Find<AMaterial>(L"Player"));

	pScene->AddObject(1, playerObject);

	// Opponent Object 추가
	pObject = new GameObject;
	pObject->SetName(L"Opponent");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	pObject->AddComponent(new CDotween);

	Ptr<CAtlasAnimator> opponentAnim = new CAtlasAnimator();
	opponentAnim->LoadMapInfo(FIND(ATexture, L"daddy"));
	pObject->AddComponent(opponentAnim.Get());

	Ptr<COpponentController> oController = new COpponentController();
	oController->SetAnimator(opponentAnim);
	pObject->AddComponent(oController.Get());
	gm->SetOpponentController(oController.Get());

	pObject->AddComponent(new CCollider2D);
	pObject->Transform()->SetRelativePosition(Vec3(-280.f, -100.f, 1.f));
	pObject->Transform()->SetRelativeScale(Vec3(300.f, 450.f, 1.f));

	pObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(AssetManager::GetInstance()->Find<AMaterial>(L"Daddy"));

	gm->SetOpponent(pObject);

	pScene->AddObject(2, pObject);

	// GirlFriend
	pObject = new GameObject;
	pObject->SetName(L"GF");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);

	Ptr<CAtlasAnimator> gfAnim = new CAtlasAnimator();
	gfAnim->LoadMapInfo(FIND(ATexture, L"gf"));
	pObject->AddComponent(gfAnim.Get());
	pObject->Transform()->SetRelativePosition(Vec3(0.f, -52.f, 2.f));
	pObject->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));

	pObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(AssetManager::GetInstance()->Find<AMaterial>(L"GF"));

	pScene->AddObject(1, pObject);


	pObject = new GameObject;
	pObject->SetName(L"EM");
	pObject->AddComponent(new CTransform);
	Ptr<CEventManager> eventManagerComp = new CEventManager;
	eventManagerComp->SetCam(camComp);
	eventManagerComp->SetPlayer(pController);
	eventManagerComp->SetOpponent(oController);

	pObject->AddComponent(eventManagerComp.Get());
	pScene->AddObject(1, pObject);
#pragma endregion

	// 광원 추가
	pObject = new GameObject;
	pObject->SetName(L"DirectionalLight");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));

	pScene->AddObject(0, pObject);

	// 광원 추가
	Vec3 dirVec;

	{
		pObject = new GameObject;
		pObject->SetName(L"PlayerSpotLight");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CLight2D);
		pObject->AddComponent(new CDotween);

		pObject->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
		pObject->Light2D()->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
		pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
		pObject->Light2D()->SetRadius(1000.f);
		pObject->Transform()->SetRelativePosition(Vec3(224.f, 370.f, 10.f));

		dirVec = playerObject->Transform()->GetRelativePosition() - pObject->Transform()->GetRelativePosition();
		dirVec.z = 0.f;
		dirVec.Normalize();

		Vec3 base(1.f, 0.f, 0.f);		// 우벡터 기준
		dirVec.Normalize();
		float dot = base.Dot(dirVec);
		float crossZ = base.Cross(dirVec).z;

		float angleZ = atan2(crossZ, dot);

		pObject->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, -90.f));
		pObject->Light2D()->SetAngle(35.f);
		pScene->AddObject(0, pObject);
	}

	{
		pObject = new GameObject;
		pObject->SetName(L"OpponentSpotLight");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CLight2D);
		pObject->AddComponent(new CDotween);

		pObject->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
		pObject->Light2D()->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
		pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
		pObject->Light2D()->SetRadius(800.f);
		pObject->Transform()->SetRelativePosition(Vec3(-266.f, 370.f, 10.f));

		dirVec = playerObject->Transform()->GetRelativePosition() - pObject->Transform()->GetRelativePosition();
		dirVec.z = 0.f;
		dirVec.Normalize();

		Vec3 base(1.f, 0.f, 0.f);		// 우벡터 기준
		dirVec.Normalize();
		float dot = base.Dot(dirVec);
		float crossZ = base.Cross(dirVec).z;

		float angleZ = atan2(crossZ, dot);

		pObject->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, -90.f));
		pObject->Light2D()->SetAngle(40.f);
		pScene->AddObject(0, pObject);
	}

	// AddChilde 이후 AddObj
	pScene->AddObject(0, camObj);
	pScene->AddObject(31, UICamObj);

	pScene->SetChanged();

	AssetManager::GetInstance()->AddAsset(pScene->GetName(), pScene.Get());
}

void CreateMainMenuScene()
{
	SongManager::GetInstance()->ChangeSong(STAGENUM::MAINMENU);
	SOUNDMANAGER->CreateResources(SONGNUM);

	// Scene 생성
	Ptr<AScene> pScene = new AScene;
	pScene->SetName(L"MainMenuScene");

	// 카메라 역할 오브젝트 
	Ptr<GameObject> camObj = new GameObject;
	camObj->SetName(L"MainCamera");

	Ptr<CCamera> camComp = new CCamera;
	camObj->AddComponent(new CTransform);
	camObj->AddComponent(camComp.Get());
	camObj->AddComponent(new CCameraMove);
	camObj->AddComponent(new CDotween);
	//camObj->AddComponent(new BeatCamera);

	camObj->Camera()->LayerCheckAll();

	camObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	camObj->Camera()->SetFar(10000.f);
	camObj->Camera()->SetFOV(90.f);
	camObj->Camera()->SetOrthoScale(1.f);
	Vec2 vResolution = Vec2(GWinSizeX, GWinSizeY);
	camObj->Camera()->SetAspectRatio(vResolution.x / vResolution.y); // 종횡비(AspectRatio)
	camObj->Camera()->SetWidth(vResolution.x);

	camObj->Transform()->SetRelativePosition(Vec3(0.f, 0.f, -100.f));

	pScene->AddObject(0, camObj);


	Ptr<GameObject> pObject = nullptr;
	pObject = new GameObject;
	pObject->SetName(L"gftitle");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	pObject->AddComponent(new CAtlasAnimator);
	pObject->AtlasAnimator()->LoadMapInfo(FIND(ATexture, L"gftitle"));
	pObject->AtlasAnimator()->Play("gfDance", true);

	pObject->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(FIND(AMaterial, L"gfTitle"));

	pObject->Transform()->SetRelativePosition(Vec3(182.f, 0.f, 0.f));
	pObject->Transform()->SetRelativeScale(Vec3(600.f, 600.f, 300.f));

	pScene->AddObject(0, pObject);

	pObject = new GameObject;
	pObject->SetName(L"logotitle");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	pObject->AddComponent(new CAtlasAnimator);
	pObject->AtlasAnimator()->LoadMapInfo(FIND(ATexture, L"logotitle"));
	pObject->AtlasAnimator()->Play("logo bumpin", true);

	pObject->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(FIND(AMaterial, L"logoTitle"));

	pObject->Transform()->SetRelativePosition(Vec3(-351.f, 113.f, 0.f));
	pObject->Transform()->SetRelativeScale(Vec3(700.f, 600.f, 300.f));

	pScene->AddObject(0, pObject);

	pObject = new GameObject;
	pObject->SetName(L"titleenter");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	pObject->AddComponent(new CAtlasAnimator);
	pObject->AtlasAnimator()->LoadMapInfo(FIND(ATexture, L"titleenter"));
	pObject->AtlasAnimator()->Play("Press Enter to Begin", true);

	pObject->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(FIND(AMaterial, L"titleEnter"));

	pObject->Transform()->SetRelativePosition(Vec3(-7.f, -262.f, -1.f));
	pObject->Transform()->SetRelativeScale(Vec3(900.f, 100.f, 300.f));

	pScene->AddObject(0, pObject);

	pObject = new GameObject;
	pObject->SetName(L"titleblink");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);

	pObject->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(FIND(AMaterial, L"titleBlink")->Clone());

	pObject->Transform()->SetRelativePosition(Vec3(0.f, 0.f, -0.1f));
	pObject->Transform()->SetRelativeScale(Vec3(2000.f, 1080.f, 300.f));

	pScene->AddObject(0, pObject);

	pObject = new GameObject;
	pObject->SetName(L"StageChanger");
	pObject->AddComponent(new CTransform);

	pObject->AddComponent(new TitleScript);
	pScene->AddObject(0, pObject);

	pScene->SetChanged();

	AssetManager::GetInstance()->AddAsset(L"MainMenuScene", pScene.Get());
}

void CreateScene(STAGENUM stage, DIFFICULTIES diff, wstring sceneName)
{
	SongManager::GetInstance()->ChangeSong(stage);
	SongManager::GetInstance()->SetDiff(diff);

	SOUNDMANAGER->CreateResources(SONGNUM);

	// Scene 생성
	Ptr<AScene> pScene = new AScene;
	pScene->SetName(sceneName);

	Ptr<GameObject> pObject = nullptr;

	// 카메라 역할 오브젝트 
	Ptr<GameObject> camObj = new GameObject;
	camObj->SetName(L"MainCamera");

	Ptr<CCamera> camComp = new CCamera;
	camComp->SetCamType(CamIdx::MAINCAM);
	camObj->AddComponent(new CTransform);
	camObj->AddComponent(camComp.Get());
	camObj->AddComponent(new CCameraMove);
	camObj->AddComponent(new CDotween);
	//camObj->AddComponent(new BeatCamera);

	camObj->Camera()->LayerCheckAll();
	camObj->Camera()->LayerCheck(31);

	camObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	camObj->Camera()->SetFar(10000.f);
	camObj->Camera()->SetFOV(90.f);
	camObj->Camera()->SetOrthoScale(1.3f);
	Vec2 vResolution = Vec2(GWinSizeX, GWinSizeY);
	camObj->Camera()->SetAspectRatio(vResolution.x / vResolution.y); // 종횡비(AspectRatio)
	camObj->Camera()->SetWidth(vResolution.x);

	camObj->Transform()->SetRelativePosition(Vec3(0.f, -60.f, -100.f));

	// HUD camera
	Ptr<GameObject> UICamObj = new GameObject;
	UICamObj->SetName(L"HUDCamera");
	UICamObj->AddComponent(new CTransform);
	UICamObj->AddComponent(new CCamera);
	UICamObj->AddComponent(new CCameraMove);
	UICamObj->Camera()->LayerCheckClear();
	UICamObj->Camera()->LayerCheck(31);

	UICamObj->Camera()->SetCamType(CamIdx::HUDCAM);
	UICamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	UICamObj->Camera()->SetFar(10000.f);
	UICamObj->Camera()->SetFOV(90.f);
	UICamObj->Camera()->SetOrthoScale(1.f);
	UICamObj->Camera()->SetAspectRatio(vResolution.x / vResolution.y); // 종횡비(AspectRatio)
	UICamObj->Camera()->SetWidth(vResolution.x);

	UICamObj->Transform()->SetRelativePosition(Vec3(0.f, 0.f, -100.f));

#pragma region FNF Contents

#pragma region Game Manager, CNote Receptors
	Ptr<CGameManager> gm = new CGameManager;
	gm->SetDifficulty(DIFF);
	gm->SetCamComp(camComp);
	Ptr<GameObject> gmObj = new GameObject();
	gmObj->SetName(L"GM");
	gmObj->AddComponent(gm.Get());
	gmObj->AddComponent(new CTransform);
	pScene->AddObject(0, gmObj);

	// 1. CNote Receptor 오브젝트 생성
	// 스프라이트 출력, 컴포넌트 세팅 등...

	// 각 NR들은 하나의 이미지를 공유한다.
	Ptr<ATexture> nrTexture = new ATexture();
	nrTexture = AssetManager::GetInstance()->Find<ATexture>(L"notereceptor");

	Vec3 opponentNRPos = Vec3(-400.f, 200.f, 20.f);
	Vec3 playerNRPos = Vec3(100.f, 200.f, 20.f);

	for (UINT i = 0; i < NOTE_SIZE; i++)
	{
		Ptr<CAtlasAnimator> animator = new CAtlasAnimator;
		animator->LoadMapInfo(nrTexture);
		Ptr<CNoteReceptor> nrCom;
		if (i < (UINT)ARROW_DIR::END)
			nrCom = new CPlayerNoteReceptor();
		else
			nrCom = new COpponentNoteReceptor();

		nrCom->SetAnimator(animator);

		gm->SetNoteReceptor(i, nrCom);
		
		Ptr<GameObject> nrObj = new GameObject();
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"NoteReceptor_%d", i);
		nrObj->SetName(Buff);
		nrObj->AddComponent(new CTransform);
		nrObj->AddComponent(new CMeshRenderer);
		nrObj->AddComponent(nrCom.Get());
		nrObj->AddComponent(animator.Get());

		nrObj->AddComponent(new CDotween);

		Ptr<AMesh> nrMesh = (AMesh*)AssetManager::GetInstance()->FindAsset(ASSET_TYPE::MESH, L"RectMesh").Get();
		Ptr<AMaterial> nrMaterial = AssetManager::GetInstance()->Find<AMaterial>(L"NoteReceptor");

		nrObj->GetMeshRenderer()->SetMesh(nrMesh);
		nrObj->GetMeshRenderer()->SetMaterial(nrMaterial);
		nrObj->GetTransform()->SetIndependentScale(true);
		nrObj->GetTransform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
		UICamObj->AddChild(nrObj);

		if (i < (UINT)ARROW_DIR::END)
		{
			nrObj->GetTransform()->SetRelativePosition(playerNRPos);
			playerNRPos.x += 100.f;
		}
		else
		{
			nrObj->GetTransform()->SetRelativePosition(opponentNRPos);
			opponentNRPos.x += 100.f;
		}

		nrCom->SetDir(ARROW_DIR(i % 4));
		nrCom->AtlasAnimator()->Play(defaultArrowName[i % 4], true);
	}

	// 곡에 대한 노트 정보를 각각의 Receptor에게 전달.
	PARSER->LoadNoteData(SONGNAME, gm->GetNoteReceptors());
#pragma endregion

#pragma region Note 생성 로직

	float noteSpeed = 500.f;
	for (size_t i = 0; i < NOTE_SIZE; ++i)
	{
		auto vec = gm->GetNoteReceptors()[i]->GetTapTimings();
		gm->GetNoteReceptors()[i]->SetNrNum(i);
		for (size_t j = 0; j < vec.size(); ++j)
		{
			Ptr<ASprite>	sprite = new ASprite;
			wstring path = L"Sprite\\" + noteNames[(UINT)ARROW_DIR(i % 4)] + L".sprite";
			sprite = LOAD(ASprite, path);     // body

			Ptr<CNote> noteCom = new CNote;
			noteCom->SetNoteSpeed(noteSpeed);
			noteCom->SetArrowDir(ARROW_DIR(i % 4));
			Ptr<GameObject> noteObj = new GameObject;
			wchar_t Buff[50] = {};
			swprintf_s(Buff, L"Note_%d%d", i, j);

			noteObj->SetName(Buff);
			noteObj->AddComponent(new CTransform);
			noteObj->AddComponent(new CSpriteRenderer);
			noteObj->AddComponent(noteCom.Get());

			noteObj->SpriteRenderer()->SetSprite(sprite);
			noteObj->SpriteRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));

			noteObj->GetTransform()->SetIndependentScale(true);
			noteObj->GetTransform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

			float localTime = vec[j] * 0.001f;
			
			float oneBar = 60.f / BPM * 5;
			float yOffset = noteSpeed * (localTime + oneBar) + noteSpeed * MAGIC_NUM;

			gm->GetNoteReceptors()[i]->GetOwner()->AddChild(noteObj);

			noteObj->GetTransform()->SetRelativePosition(Vec3(0.f, -yOffset, 0.f));
		}

		const auto& holdMap = gm->GetNoteReceptors()[i]->GetPressTimings();
		int cnt = 0;

		for (auto& pair : holdMap)
		{
			float startTime = pair.first;     // ms
			float holdMs = pair.second;       // ms

			float holdTimeSec = holdMs * 0.001f;
			float holdPixelLength = noteSpeed * holdTimeSec;

			Ptr<ASprite>	sprite = new ASprite;
			Ptr<GameObject> barObj = new GameObject;

			wstring path = L"Sprite\\" + noteHoldNames[(UINT)ARROW_DIR(i % 4) * 2] + L".sprite";
			sprite = LOAD(ASprite, path);     // body

			barObj->AddComponent(new CTransform);
			barObj->AddComponent(new CSpriteRenderer);
			wchar_t Buff[50] = {};
			swprintf_s(Buff, L"Body_%d%d", i, cnt);

			barObj->SetName(Buff);
			Ptr<CNote> noteBody = new CNote;
			noteBody->IsBody(true);
			noteBody->SetNoteSpeed(noteSpeed);
			noteBody->SetArrowDir(ARROW_DIR(i % 4));
			barObj->AddComponent(noteBody.Get());

			barObj->SpriteRenderer()->SetSprite(sprite);
			barObj->SpriteRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));

			barObj->GetTransform()->SetIndependentScale(true);
			barObj->GetTransform()->SetRelativeScale(Vec3(35.f, holdPixelLength, 1.f));
			gm->GetNoteReceptors()[i]->GetOwner()->AddChild(barObj);
		
			{
				Ptr<ASprite>	sprite = new ASprite;
				Ptr<GameObject> barObj = new GameObject;

				wchar_t Buff[50] = {};
				swprintf_s(Buff, L"Tail_%d%d", i, cnt);

				barObj->SetName(Buff);

				wstring path = L"Sprite\\" + noteHoldNames[(UINT)ARROW_DIR(i % 4) * 2 + 1] + L".sprite";
				sprite = LOAD(ASprite, path);     // tail

				barObj->AddComponent(new CTransform);
				barObj->AddComponent(new CSpriteRenderer);

				Ptr<CNote> noteTail = new CNote;
				noteTail->IsBody(true);
				noteTail->SetNoteSpeed(noteSpeed);
				noteTail->SetArrowDir(ARROW_DIR(i % 4));
				barObj->AddComponent(noteTail.Get());

				barObj->SpriteRenderer()->SetSprite(sprite);
				barObj->SpriteRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));

				barObj->GetTransform()->SetIndependentScale(true);
				barObj->GetTransform()->SetRelativeScale(Vec3(35.f, 100.f, 1.f));
				gm->GetNoteReceptors()[i]->GetOwner()->AddChild(barObj);
			}
			cnt++;
		}
	}

#pragma endregion

	// Player Object 추가
	Ptr<GameObject> playerObject = new GameObject;
	playerObject->SetName(L"Player");
	playerObject->AddComponent(new CTransform);
	playerObject->AddComponent(new CMeshRenderer);
	//pObject->AddComponent(new CPlayerScript);

	Ptr<CAtlasAnimator> playerAnim = new CAtlasAnimator();
	playerAnim->LoadMapInfo(FIND(ATexture, L"bf"));
	playerObject->AddComponent(playerAnim.Get());

	Ptr<CPlayerController> pController = new CPlayerController();
	pController->SetAnimator(playerAnim);
	playerObject->AddComponent(pController.Get());
	gm->SetPlayerController(pController.Get());

	playerObject->AddComponent(new CCollider2D);
	playerObject->Transform()->SetRelativePosition(Vec3(216.f, -193.f, 1.f));
	playerObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	playerObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	playerObject->MeshRenderer()->SetMaterial(AssetManager::GetInstance()->Find<AMaterial>(L"Player"));

	pScene->AddObject(1, playerObject);

	// Opponent Object 추가
	Ptr<GameObject> opponentObject = new GameObject;
	opponentObject->SetName(L"Opponent");
	opponentObject->AddComponent(new CTransform);
	opponentObject->AddComponent(new CMeshRenderer);
	opponentObject->AddComponent(new CDotween);

	Ptr<CAtlasAnimator> opponentAnim = new CAtlasAnimator();
	opponentAnim->LoadMapInfo(FIND(ATexture, L"daddy"));
	opponentObject->AddComponent(opponentAnim.Get());

	Ptr<COpponentController> oController = new COpponentController();
	oController->SetAnimator(opponentAnim);
	opponentObject->AddComponent(oController.Get());
	gm->SetOpponentController(oController.Get());

	opponentObject->AddComponent(new CCollider2D);
	opponentObject->Transform()->SetRelativePosition(Vec3(-280.f, -100.f, 1.f));
	opponentObject->Transform()->SetRelativeScale(Vec3(300.f, 450.f, 1.f));

	opponentObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	opponentObject->MeshRenderer()->SetMaterial(AssetManager::GetInstance()->Find<AMaterial>(L"Daddy"));

	gm->SetOpponent(opponentObject);

	pScene->AddObject(2, opponentObject);

	// GirlFriend
	pObject = new GameObject;
	pObject->SetName(L"GF");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);

	Ptr<CAtlasAnimator> gfAnim = new CAtlasAnimator();
	gfAnim->LoadMapInfo(FIND(ATexture, L"gf"));
	pObject->AddComponent(gfAnim.Get());
	pObject->Transform()->SetRelativePosition(Vec3(0.f, -52.f, 2.f));
	pObject->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));

	pObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(AssetManager::GetInstance()->Find<AMaterial>(L"GF"));

	pScene->AddObject(1, pObject);

	pObject = new GameObject;
	pObject->SetName(L"RSG");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	Ptr<CStartingPistol> rsgCom = new CStartingPistol;
	rsgCom->SetBPM(BPM);
	rsgCom->SetGM(gm);
	pObject->AddComponent(rsgCom.Get());
	pObject->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(FIND(AMaterial, L"RSG")->Clone());
	pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	UICamObj->AddChild(pObject);
	pObject->Transform()->SetRelativePosition(Vec3(0.f, 0.f, 1.f));
	//pScene->AddObject(1, pObject);

	pObject = new GameObject;
	pObject->SetName(L"EM");
	pObject->AddComponent(new CTransform);
	Ptr<CEventManager> eventManagerComp = new CEventManager;
	eventManagerComp->SetCam(camComp);
	eventManagerComp->SetPlayer(pController);
	eventManagerComp->SetOpponent(oController);
	rsgCom->SetEM(eventManagerComp);

	pObject->AddComponent(eventManagerComp.Get());
	pScene->AddObject(1, pObject);
#pragma endregion

#pragma region BackGrounds
	pObject = new GameObject;
	pObject->SetName(L"StageFront");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	pObject->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(FIND(AMaterial, L"StageFront"));

	pObject->Transform()->SetRelativeScale(Vec3(2000.f, 600.f, 1.f));
	pObject->Transform()->SetRelativePosition(Vec3(-11.f, -411.f, 100.f));
	pScene->AddObject(1, pObject);

	pObject = new GameObject;
	pObject->SetName(L"StageBack");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	pObject->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(FIND(AMaterial, L"StageBack"));

	pObject->Transform()->SetRelativeScale(Vec3(2000.f, 900.f, 1.f));
	pObject->Transform()->SetRelativePosition(Vec3(0.f, 0.f, 100.f));
	pScene->AddObject(1, pObject);

#pragma endregion


	// 광원 추가
	pObject = new GameObject;
	pObject->SetName(L"DirectionalLight");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->Light2D()->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
	//pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));

	pScene->AddObject(0, pObject);

	// 광원 추가
	Vec3 dirVec;

	{
		pObject = new GameObject;
		pObject->SetName(L"PlayerSpotLight");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CLight2D);
		pObject->AddComponent(new CDotween);

		pObject->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
		pObject->Light2D()->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
		pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
		pObject->Light2D()->SetRadius(1000.f);
		pObject->Transform()->SetRelativePosition(Vec3(224.f, 370.f, 10.f));

		dirVec = playerObject->Transform()->GetRelativePosition() - pObject->Transform()->GetRelativePosition();
		dirVec.z = 0.f;
		dirVec.Normalize();

		Vec3 base(1.f, 0.f, 0.f);		// 우벡터 기준
		dirVec.Normalize();
		float dot = base.Dot(dirVec);
		float crossZ = base.Cross(dirVec).z;

		float angleZ = atan2(crossZ, dot);

		pObject->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, angleZ));
		pObject->Light2D()->SetAngle(35.f);
		pScene->AddObject(0, pObject);
	}

	{
		pObject = new GameObject;
		pObject->SetName(L"OpponentSpotLight");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CLight2D);
		pObject->AddComponent(new CDotween);

		pObject->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
		pObject->Light2D()->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
		pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
		pObject->Light2D()->SetRadius(800.f);
		pObject->Transform()->SetRelativePosition(Vec3(-266.f, 370.f, 10.f));

		dirVec = opponentObject->Transform()->GetRelativePosition() - pObject->Transform()->GetRelativePosition();
		dirVec.z = 0.f;
		dirVec.Normalize();

		Vec3 base(1.f, 0.f, 0.f);		// 우벡터 기준
		dirVec.Normalize();
		float dot = base.Dot(dirVec);
		float crossZ = base.Cross(dirVec).z;

		float angleZ = atan2(crossZ, dot);

		pObject->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, angleZ));
		pObject->Light2D()->SetAngle(40.f);
		pScene->AddObject(0, pObject);
	}

/*	// 광원 추가
	pObject = new GameObject;
	pObject->SetName(L"PlayerSpotLight");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pObject->Light2D()->SetLightColor(Vec3(1.f, 0.f, 0.f));
	pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
	pObject->Light2D()->SetRadius(800.f);
	pObject->Transform()->SetRelativePosition(Vec3(-200.f, 400.f, 10.f));

	dirVec = playerObject->Transform()->GetRelativePosition() - pObject->Transform()->GetRelativePosition();

	dirVec.z = 0.f;
	dirVec.Normalize();
	Vec3 base(1.f, 0.f, 0.f);
	float dot = base.Dot(dirVec);
	float crossZ = base.Cross(dirVec).z;

	float angleZ = atan2(crossZ, dot);

	pObject->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, angleZ));
	pObject->Light2D()->SetAngle(40.f);

	pScene->AddObject(0, pObject);*/

	// 광원 추가
/*	pObject = new GameObject;
	pObject->SetName(L"Light3");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pObject->Light2D()->SetLightColor(Vec3(0.f, 0.f, 1.f));
	pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
	pObject->Light2D()->SetRadius(800.f);
	pObject->Transform()->SetRelativePosition(Vec3(500.f, 400.f, 10.f));

	dirVec = playerObject->Transform()->GetRelativePosition() - pObject->Transform()->GetRelativePosition();

	dirVec.z = 0.f;
	dirVec.Normalize();
	dot = base.Dot(dirVec);
	crossZ = base.Cross(dirVec).z;

	angleZ = atan2(crossZ, dot);

	pObject->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, angleZ));
	pObject->Light2D()->SetAngle(40.f);

	pScene->AddObject(0, pObject);*/

	pScene->AddObject(0, camObj);
	pScene->AddObject(31, UICamObj);


	pObject = new GameObject;
	pObject->SetName(L"StageChanger");
	pObject->AddComponent(new CTransform);

	pObject->AddComponent(new CStageTransition());
	pScene->AddObject(0, pObject);
	// Tile Object
/*	Ptr<GameObject> pTileObj = new GameObject;

	pTileObj->AddComponent(new CTransform);
	pTileObj->AddComponent(new CCollider2D);
	pTileObj->AddComponent(new CTileRenderer);

	pTileObj->Transform()->SetRelativePosition(Vec3(-640.f, 640.f, 500.f));
	pTileObj->TileRenderer()->SetTileMap(LOAD(ATileMap, L"TileMap\\TestTileMap.tile"));

	pScene->AddObject(2, pTileObj);*/

	pScene->GetLayer(0)->SetName(L"Default");
	pScene->GetLayer(1)->SetName(L"Player");
	pScene->GetLayer(2)->SetName(L"Monster");
	pScene->GetLayer(3)->SetName(L"BackGround");
	pScene->GetLayer(4)->SetName(L"Etc");

	pScene->CheckCollisionLayer(1, 2);
	//pScene->CheckCollisionLayer(1, 4);
	//pScene->CheckCollisionLayer(0, 1);

	pScene->SetChanged();

	AssetManager::GetInstance()->AddAsset(sceneName, pScene.Get());

	//ChangeScene(L"TestScene");
}

void CreateLoadingScene(STAGENUM nextStage, wstring nextSceneName)
{
	// Scene 생성
	Ptr<AScene> pScene = new AScene;
	wstring loadingSceneName = L"Loading_" + nextSceneName;
	pScene->SetName(loadingSceneName);

	Ptr<GameObject> camObj = new GameObject;
	camObj->SetName(L"MainCamera");

	Ptr<CCamera> camComp = new CCamera;
	camComp->SetCamType(CamIdx::MAINCAM);
	camObj->AddComponent(new CTransform);
	camObj->AddComponent(camComp.Get());
	//camObj->AddComponent(new CDotween);
	//camObj->AddComponent(new BeatCamera);

	camObj->Camera()->LayerCheckAll();

	camObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	camObj->Camera()->SetFar(10000.f);
	camObj->Camera()->SetFOV(90.f);
	camObj->Camera()->SetOrthoScale(1.f);
	Vec2 vResolution = Vec2(GWinSizeX, GWinSizeY);
	camObj->Camera()->SetAspectRatio(vResolution.x / vResolution.y); // 종횡비(AspectRatio)
	camObj->Camera()->SetWidth(vResolution.x);

	camObj->Transform()->SetRelativePosition(Vec3(0.f, 0.f, -1.f));
	pScene->AddObject(0, camObj);

	// Loading Image하나와 loading script를 생성하여 Scene을 동적 생성 후 load하도록 하기

	// Loading Image
	Ptr<GameObject> pObject = nullptr;
	pObject = new GameObject;
	pObject->SetName(L"LoadingImg");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);

	pObject->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRenderer()->SetMaterial(FIND(AMaterial, L"Loading"));

	pObject->Transform()->SetRelativePosition(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetRelativeScale(Vec3(GWinSizeX, GWinSizeY, 1.f));

	pScene->AddObject(0, pObject);

	// Loading script
	pObject = new GameObject;
	pObject->SetName(L"StageLoader");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CStageLoader(nextStage, nextSceneName));

	pScene->AddObject(0, pObject);

	pScene->SetChanged();

	AssetManager::GetInstance()->AddAsset(loadingSceneName, pScene.Get());
}
