#include "pch.h"
#include "SceneManager.h"
#include "AMesh.h"
#include "AGrapicShader.h"
#include "ATexture.h"
#include "GameObject.h"
#include "ConstantBuffer.h"
#include "CPlayerScript.h"
#include "COpponentController.h"
#include "AssetManager.h"
#include "CAtlasAnimator.h"
#include "CImage.h"
#include "Dotween.h"
#include "Easing.h"
#include "GameManager.h"
#include "CPlayerController.h"
#include "CCameraMove.h"
#include "StartingPistol.h"
#include "CollisionManager.h"
#include "EventManager.h"

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	// Scene 생성
	m_currentScene = new AScene;
	m_currentScene->SetName(L"Current Scene");

#pragma region Camera
	//Ptr<GameObject> camObj = new GameObject;
	//camObj->SetName(L"Main Camera");

	//camObj->AddComponent(new CTransform);
	//camObj->AddComponent(new CCamera);
	//camObj->AddComponent(new CCameraMove);
	//auto cam = (CCamera*)camObj->GetComponent(COMPONENT_TYPE::CAMERA).Get();
	//cam->SetProjType(PROJ_TYPE::PERSPECTIVE);
	//cam->SetFar(5000.f);
	//cam->SetWidth(GWinSizeX);
	//cam->SetFOV(90.f);
	//cam->SetOrthoScale(1.f);
	//cam->SetAspectRatio(GWinSizeX / GWinSizeY);
	//cam->LayerCheckAll();
	//
	//camObj->GetTransform()->SetRelativePosition(Vec3(0.f, 0.f, -30.f));

	//m_currentScene->AddObject(0, camObj);
#pragma endregion

#pragma region Sun
	//Ptr<ATexture> sunTexture = new ATexture();
	//sunTexture->Load(L"\\SolarSystem\\Sun.jpg");

	//Ptr<CImage> sunImage = new CImage;
	//sunImage->LoadMapInfo(sunTexture);

	//Ptr<GameObject> sunObj = new GameObject();
	//sunObj->AddComponent(new CTransform);
	//sunObj->AddComponent(new CMeshRenderer);
	//sunObj->AddComponent(sunImage.Get());

	//sunObj->AddComponent(new SolarSystem(cam));

	//Ptr<AMesh> sumMesh = (AMesh*)AssetManager::GetInstance()->FindAsset(ASSET_TYPE::MESH, L"RectMesh2").Get();
	//Ptr<AGraphicShader> sunShader = AssetManager::GetInstance()->Find<AGraphicShader>(L"Shader2");

	//sunObj->GetMeshRenderer()->SetMesh(sumMesh);
	//sunObj->GetMeshRenderer()->SetShader(sunShader);
	//sunObj->GetMeshRenderer()->LoadMapInfo(sunTexture);

	//sunObj->GetTransform()->SetRelativePosition(Vec3(0.f, 0.f, 10.f));
	//sunObj->GetTransform()->SetRelativeScale(Vec3(300.f, 300.f, 300.f));
	//
	//m_currentScene->AddObject(0, sunObj);
#pragma endregion

#pragma region Earth
	//Ptr<ATexture> earthTexture = new ATexture();
	//earthTexture->Load(L"\\SolarSystem\\Earth.jpg");

	//Ptr<CImage> earthImage = new CImage;
	//earthImage->LoadMapInfo(earthTexture);

	//Ptr<GameObject> earthObj = new GameObject();
	//earthObj->AddComponent(new CTransform);
	//earthObj->AddComponent(new CMeshRenderer);
	//earthObj->AddComponent(earthImage.Get());

	//Ptr<AMesh> earthMesh = (AMesh*)AssetManager::GetInstance()->FindAsset(ASSET_TYPE::MESH, L"RectMesh2").Get();
	//Ptr<AGraphicShader> earthShader = AssetManager::GetInstance()->Find<AGraphicShader>(L"Shader2");

	//earthObj->GetMeshRenderer()->SetMesh(earthMesh);
	//earthObj->GetMeshRenderer()->SetShader(earthShader);
	//earthObj->GetMeshRenderer()->LoadMapInfo(earthTexture);

	//earthObj->GetTransform()->SetRelativePosition(Vec3(-500.f, 0.f, 10.f));
	//earthObj->GetTransform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

	//m_currentScene->AddObject(0, earthObj);
#pragma endregion

#pragma region Character Objects
	//Ptr<CPlayerController> playerController = new CPlayerController(L"bf");
	//Ptr<GameObject> player = new GameObject();
	//player->AddComponent(playerController.Get());
	//playerController->Init();
	//player->GetTransform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
	//player->GetTransform()->SetRelativePosition(Vec3(300.f, 0.f, 10.f));
	//gm->SetPlayerController(playerController.Get());

	//Ptr<COpponentController> opponentController = new COpponentController(L"daddy");
	//Ptr<GameObject> opponent = new GameObject();
	//opponent->AddComponent(opponentController.Get());
	//opponentController->Init();
	//opponent->GetTransform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
	//opponent->GetTransform()->SetRelativePosition(Vec3(-300.f, 0.f, 10.f));
	//gm->SetOpponentController(opponentController.Get());
#pragma endregion

#pragma region BackGround Images
	//// Stage
	//Ptr<ATexture> bgTexture = new ATexture();
	//bgTexture->Load(L"\\bg\\stageback.png");

	////Ptr<CImage> bgImg = new CImage;
	////bgImg->LoadMapInfo(bgTexture);

	//Ptr<GameObject> bgObj = new GameObject();
	//bgObj->AddComponent(new CTransform);
	//bgObj->AddComponent(new CMeshRenderer);
	////bgObj->AddComponent(bgImg.Get());

	//Ptr<AMesh> bgMesh = (AMesh*)AssetManager::GetInstance()->FindAsset(ASSET_TYPE::MESH, L"RectMesh2").Get();
	//Ptr<AGraphicShader> bgShader = AssetManager::GetInstance()->Find<AGraphicShader>(L"Shader2");

	//Ptr<AMaterial> bgMat = new AMaterial;
	//bgMat->LoadMapInfo(TEX_0, bgTexture);
	//bgMat->SetShader(bgShader);

	//bgObj->GetMeshRenderer()->SetMesh(bgMesh);
	//bgObj->GetMeshRenderer()->SetMtrl(bgMat);

	//bgObj->GetTransform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	//bgObj->GetTransform()->SetRelativePosition(Vec3(0.f, -500.f, 1.f));

	//m_currentScene->AddObject(0, bgObj);

	//// Stage
	//Ptr<ATexture> stageFrontTexture = new ATexture();
	//stageFrontTexture->Load(L"\\bg\\stagefront.png");

	//Ptr<CImage> stageFrontImg = new CImage;
	//stageFrontImg->LoadMapInfo(stageFrontTexture);

	//Ptr<GameObject> stageFrontObj = new GameObject();
	//stageFrontObj->SetName(L"StageFront");

	//stageFrontObj->AddComponent(new CTransform);
	//stageFrontObj->AddComponent(new CMeshRenderer);
	//stageFrontObj->AddComponent(stageFrontImg.Get());

	//Ptr<AMesh> stageFrontMesh = (AMesh*)AssetManager::GetInstance()->FindAsset(ASSET_TYPE::MESH, L"RectMesh3").Get();
	//Ptr<AGraphicShader> stageFrontShader = AssetManager::GetInstance()->Find<AGraphicShader>(L"Shader2");

	//stageFrontObj->GetMeshRenderer()->SetMesh(stageFrontMesh);

	//stageFrontObj->GetTransform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	//stageFrontObj->GetTransform()->SetRelativePosition(Vec3(0.f, 500.f, 1.f));

	//m_currentScene->AddObject(0, stageFrontObj);
#pragma endregion
	
	Ptr<GameObject> pObject = nullptr;

	// 카메라 역할 오브젝트 
	Ptr<GameObject> camObj = new GameObject;
	camObj->SetName(L"MainCamera");

	Ptr<CCamera> camComp = new CCamera;
	camObj->AddComponent(new CTransform);
	camObj->AddComponent(camComp.Get());
	camObj->AddComponent(new CCameraMove);
	Ptr<Dotween> camDotween = new Dotween;
	camObj->AddComponent(camDotween.Get());
	camObj->m_dotween = camDotween.Get();

	camObj->Camera()->LayerCheckAll();

	camObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	camObj->Camera()->SetFar(10000.f);
	camObj->Camera()->SetFOV(90.f);
	camObj->Camera()->SetOrthoScale(1.f);
	Vec2 vResolution = Vec2(GWinSizeX, GWinSizeY);
	camObj->Camera()->SetAspectRatio(vResolution.x / vResolution.y); // 종횡비(AspectRatio)
	camObj->Camera()->SetWidth(vResolution.x);

	camObj->Transform()->SetRelativePosition(Vec3(-100.f, 0.f, -100.f));

	m_currentScene->AddObject(0, camObj);

#pragma region FNF Contents

#pragma region Game Manager, Note Receptors
	Ptr<GameManager> gm = new GameManager;
	Ptr<GameObject> gmObj = new GameObject();
	gmObj->AddComponent(gm.Get());
	gmObj->AddComponent(new CTransform);
	m_currentScene->AddObject(0, gmObj);
	gm->Init(DIFFICULTIES::HARD, camComp);			// NoteReceptor 제작도 해줍니다
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
	
	Ptr<CPlayerController> pController = new CPlayerController(playerAnim);
	playerObject->AddComponent(pController.Get());
	gm->SetPlayerController(pController.Get());

	playerObject->AddComponent(new CCollider2D);
	playerObject->Transform()->SetRelativePosition(Vec3(150.f, -100.f, 0.f));
	playerObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	playerObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	playerObject->MeshRenderer()->SetMtrl(AssetManager::GetInstance()->Find<AMaterial>(L"Player"));

	m_currentScene->AddObject(1, playerObject);

	// Opponent Object 추가
	pObject = new GameObject;
	pObject->SetName(L"Opponent");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	Ptr<Dotween> dotweenCom = new Dotween;
	pObject->AddComponent(dotweenCom.Get());
	pObject->m_dotween = dotweenCom.Get();

	Ptr<CAtlasAnimator> opponentAnim = new CAtlasAnimator();
	opponentAnim->LoadMapInfo(FIND(ATexture, L"daddy"));
	pObject->AddComponent(opponentAnim.Get());

	Ptr<COpponentController> oController = new COpponentController(opponentAnim);
	pObject->AddComponent(oController.Get());
	gm->SetOpponentController(oController.Get());

	pObject->AddComponent(new CCollider2D);
	pObject->Transform()->SetRelativePosition(Vec3(-350.f, -100.f, 0.f));
	pObject->Transform()->SetRelativeScale(Vec3(300.f, 450.f, 1.f));

	pObject->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	pObject->MeshRenderer()->SetMtrl(AssetManager::GetInstance()->Find<AMaterial>(L"Daddy"));
	
	gm->SetOpponent(pObject);
	gm->SetOpponent(pObject);

	m_currentScene->AddObject(2, pObject);

	pObject = new GameObject;
	pObject->SetName(L"RSG");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRenderer);
	Ptr<StartingPistol> rsgCom = new StartingPistol;
	rsgCom->SetBPM(BOPEEBO_BPM);
	rsgCom->SetGM(gm);
	pObject->AddComponent(rsgCom.Get());
	pObject->MeshRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRenderer()->SetMtrl(FIND(AMaterial, L"RSG"));
	pObject->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	
	//camObj->AddChild(pObject);
	pObject->Transform()->SetRelativePosition(Vec3(-100.f, 0.f, 1.f));
	m_currentScene->AddObject(1, pObject);

	pObject = new GameObject;
	pObject->SetName(L"EventManager");
	pObject->AddComponent(new CTransform);
	Ptr<EventManager> eventManagerComp = new EventManager;
	eventManagerComp->SetCam(camComp);
	eventManagerComp->SetPlayer(pController);
	eventManagerComp->SetOpponent(oController);
	rsgCom->SetEM(eventManagerComp);

	pObject->AddComponent(eventManagerComp.Get());
	m_currentScene->AddObject(1, pObject);
#pragma endregion

	// 광원 추가
	pObject = new GameObject;
	pObject->SetName(L"Light");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->Light2D()->SetLightColor(Vec3(0.5f, 0.5f, 0.5f));
	pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));

	m_currentScene->AddObject(0, pObject);

	// 광원 추가
	Vec3 dirVec;

	pObject = new GameObject;
	pObject->SetName(L"Light1");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pObject->Light2D()->SetLightColor(Vec3(0.f, 1.f, 0.f));
	pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
	pObject->Light2D()->SetRadius(800.f);
	pObject->Transform()->SetRelativePosition(Vec3(150.f, 400.f, 10.f));

	dirVec = playerObject->Transform()->GetRelativePosition() - pObject->Transform()->GetRelativePosition();
	dirVec.z = 0.f;
	dirVec.Normalize();

	Vec3 base(1.f, 0.f, 0.f);		// 우벡터 기준
	dirVec.Normalize();
	float dot = base.Dot(dirVec);
	float crossZ = base.Cross(dirVec).z;

	float angleZ = atan2(crossZ, dot);

	pObject->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, angleZ));
	pObject->Light2D()->SetAngle(40.f);
	m_currentScene->AddObject(0, pObject);

	// 광원 추가
	pObject = new GameObject;
	pObject->SetName(L"Light2");
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
	dot = base.Dot(dirVec);
	crossZ = base.Cross(dirVec).z;

	angleZ = atan2(crossZ, dot);

	pObject->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, angleZ));
	pObject->Light2D()->SetAngle(40.f);

	m_currentScene->AddObject(0, pObject);

	// 광원 추가
	pObject = new GameObject;
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

	m_currentScene->AddObject(0, pObject);

#pragma region 수업 컨텐츠
	//// 몬스터 생성
	//Ptr<GameObject> pMonster = new GameObject;
	//pMonster->SetName(L"Monster");

	//pMonster->AddComponent(new CTransform);
	//pMonster->AddComponent(new CSpriteRenderer);
	//pMonster->AddComponent(new CCollider2D);

	//pMonster->Transform()->SetRelativePosition(Vec3(300.f, 0.f, 100.f));
	//pMonster->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 0.f));
	//pMonster->SpriteRenderer()->SetSprite(FIND(ASprite, L"TileSprite_47"));

	//m_currentScene->AddObject(5, pMonster);

	//// Player Object 추가
	//pObject = new GameObject;
	//pObject->SetName(L"Player");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CFlipbookRenderer);
	//pObject->AddComponent(new CCollider2D);

	//Ptr<CPlayerScript> pPlayerScript = new CPlayerScript;
	////pPlayerScript->SetTarget(pMonster);
	//pObject->AddComponent(pPlayerScript.Get());

	//pObject->Transform()->SetRelativePosition(Vec3(0.f, 0.f, 100.f));
	//pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	//pObject->Collider2D()->SetOffset(Vec2(0.f, -0.1f));
	//pObject->Collider2D()->SetScale(Vec2(0.25f, 0.5f));

	//pObject->FlipbookRenderer()->AddFlipbook(FIND(AFlipbook, L"Link_MoveDown"));
	//pObject->FlipbookRenderer()->AddFlipbook(FIND(AFlipbook, L"Link_MoveLeft"));
	//pObject->FlipbookRenderer()->AddFlipbook(FIND(AFlipbook, L"Link_MoveUp"));
	//pObject->FlipbookRenderer()->AddFlipbook(FIND(AFlipbook, L"Link_MoveRight"));
	//pObject->FlipbookRenderer()->Play(1, 15.f, 10);

	//Ptr<GameObject> pChild = new GameObject;
	//pChild->SetName(L"Child");
	//pChild->AddComponent(new CTransform);
	//pChild->AddComponent(new CMeshRenderer);
	//pChild->AddComponent(new CCollider2D);

	//pChild->Transform()->SetRelativePosition(Vec3(-200.f, 0.f, 0.f));
	//pChild->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	//pChild->Transform()->SetIndependentScale(true);

	//pChild->MeshRenderer()->SetMesh(AssetManager::GetInstance()->Find<AMesh>(L"RectMesh"));
	//pChild->MeshRenderer()->SetMtrl(AssetManager::GetInstance()->Find<AMaterial>(L"Std2DMtrl"));

	//// Player 와 Child 부모자식 연결
	//pObject->AddChild(pChild);

	//// Player(부모 오브젝트) 를 레벨에 추가
	//m_currentScene->AddObject(3, pObject);


	// Tile Object
	Ptr<GameObject> pTileObj = new GameObject;

	pTileObj->AddComponent(new CTransform);
	pTileObj->AddComponent(new CCollider2D);
	pTileObj->AddComponent(new CTileRenderer);

	pTileObj->Transform()->SetRelativePosition(Vec3(-800.f, 500.f, 500.f));
	pTileObj->TileRenderer()->SetTileMap(FIND(ATileMap, L"TestTileMap"));

	m_currentScene->AddObject(2, pTileObj);
#pragma endregion

	m_currentScene->GetLayer(0)->SetName(L"Default");
	m_currentScene->GetLayer(1)->SetName(L"Player");
	m_currentScene->GetLayer(2)->SetName(L"Monster");
	m_currentScene->GetLayer(3)->SetName(L"BackGround");
	m_currentScene->GetLayer(4)->SetName(L"Etc");

	m_currentScene->CheckCollisionLayer(1, 2);
	//m_currentScene->CheckCollisionLayer(1, 4);
	//m_currentScene->CheckCollisionLayer(0, 1);

	m_currentScene->Begin();
}

void SceneManager::Progress()
{
	// 이전에 등록된 모든 오브젝트들 제거
	m_currentScene->Deregister();

	m_currentScene->Tick();			// ex) Set Transform

	m_currentScene->FinalTick();	// ex) multiply World Matrix 

	CollisionManager::GetInstance()->Progress(m_currentScene);
}
