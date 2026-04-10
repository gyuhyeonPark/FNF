#include "pch.h"
#include "TitleScript.h"
#include "SceneManager.h"
#include "SongManager.h"
#include "Source\Scripts\CDotween.h"

TitleScript::TitleScript()
	: CStageTransition()
{
}

TitleScript::~TitleScript()
{
}

void TitleScript::Begin()
{
	SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"gftitle")->AtlasAnimator()
		->Play("gfDance", true);

	SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"logotitle")->AtlasAnimator()
		->Play("logo bumpin", true);

	SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"titleenter")->AtlasAnimator()
		->Play("Press Enter to Begin", true);

	Ptr<GameObject> obj = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"titleblink");
	obj->MeshRenderer()->GetMaterial()->SetScalar(FLOAT_0, m_duration);
	obj->MeshRenderer()->GetMaterial()->SetScalar(FLOAT_1, m_duration);

	SOUNDMANAGER->ChangeStage(SONGNUM);

	SongManager::GetInstance()->SetDiff(DIFFICULTIES::HARD);
}

void TitleScript::Tick()
{
	CStageTransition::Tick();

	if (KEY_TAP(KEY::ENTER))
	{
		m_trigger = true;
		Ptr<GameObject> obj = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"titleenter");
		obj->AtlasAnimator()->Play("ENTER PRESSED", true);
		SOUNDMANAGER->PlaySFX(SoundType::MAINENTER);
	}	

	// 화면 blinck
	if (m_trigger)
	{
		Ptr<GameObject> obj = SceneManager::GetInstance()->GetCurrentScene()->FindObjectByName(L"titleblink");
		obj->MeshRenderer()->GetMaterial()->SetScalar(FLOAT_0, m_elapsed);
	}


	// 난이도 설정
	if (KEY_TAP(KEY::F1))
		SongManager::GetInstance()->SetDiff(DIFFICULTIES::EASY);

	if (KEY_TAP(KEY::F2))
		SongManager::GetInstance()->SetDiff(DIFFICULTIES::NORMAL);

	if (KEY_TAP(KEY::F3))
		SongManager::GetInstance()->SetDiff(DIFFICULTIES::HARD);
}


