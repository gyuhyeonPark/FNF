#include "pch.h"
#include "NoteReceptor.h"
#include "Note.h"
#include "AssetManager.h"
#include "CAtlasAnimator.h"
#include "SceneManager.h"
#include "AssetKeys.h"

NoteReceptor::NoteReceptor()
	: m_elapsed(0.f), m_tapTimingIdx(0), m_pressedTimingIdx(0.f),
	m_isTapped(false),m_isMusicStart(false), m_difficulty(DIFFICULTIES::EASY),
	m_noteSpeed(500.f)
{

}

NoteReceptor::~NoteReceptor()
{

}

void NoteReceptor::Begin()
{
	// m_timings의 정보에 따라 Note를 생성 및 배치한다.
	Ptr<ATexture> noteTexture = new ATexture;
	noteTexture = FIND(ATexture, L"notes");

	const vector<float>& vec = tapTimings[int(m_difficulty)];

	float timeOffset = 60.f / BOPEEBO_BPM * 4.f + 0.2f;	// 한마디 RSG offset. 거 = 속 * (시 + offset)

	for (int i = 0; i < vec.size(); ++i)
	{
		// vec[i] = 시간.
		Ptr<CAtlasAnimator> animator = new CAtlasAnimator;
		animator->LoadMapInfo(noteTexture);
		Ptr<Note> noteCom = new Note(m_noteSpeed, m_dir);
		noteCom->m_anim = animator;
		GameObject* noteObj = new GameObject;
		noteObj->SetName(L"Note");
		noteObj->AddComponent(new CTransform);
		noteObj->AddComponent(new CMeshRenderer);
		noteObj->AddComponent(animator.Get());
		noteObj->AddComponent(noteCom.Get());

		Ptr<AMesh> noteMesh = FIND(AMesh, L"RectMesh");
		Ptr<AMaterial> noteMtrl = FIND(AMaterial, L"Notes");
		noteObj->GetMeshRenderer()->SetMesh(noteMesh);
		noteObj->GetMeshRenderer()->SetMtrl(noteMtrl);

		noteObj->GetTransform()->SetIndependentScale(true);
		noteObj->GetTransform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

		float localTime = vec[i] * 0.001f + timeOffset;

		float yOffset = m_noteSpeed * localTime + 200.f;

		GetOwner()->AddChild(noteObj);

		noteObj->GetTransform()->SetRelativePosition(Vec3(0.f, -yOffset, 0.f));

		//SceneManager::GetInstance()->GetCurrentScene()->AddObject(1, noteObj);

		//CreateObject(noteObj, 1);
	}

	// NoteBar에 대해서도 해당 Bar의 길이에 따라 스프라이트를 생성해준다.
	// hold sprite

	vector<pair<float, float>> holdMap = pressTimings[int(m_difficulty)];

	for (auto& pair : holdMap)
	{
		float startTime = pair.first;     // ms
		float holdMs = pair.second;       // ms

		float holdTimeSec = holdMs * 0.001f;
		float holdPixelLength = m_noteSpeed * holdTimeSec;

		float spriteHeight = 100.f; // 현재 scale 기준
		int spriteCount = (int)ceil(holdPixelLength / spriteHeight);

		for (int j = 1; j < spriteCount; j++)
		{
			Ptr<ASprite> sprite = new ASprite;
			Ptr<GameObject> barObj = new GameObject;

			if (j == spriteCount - 1)
				sprite = FIND(ASprite, noteHoldNames[int(m_dir) * 2 + 1]); // end
			else
				sprite = FIND(ASprite, noteHoldNames[int(m_dir) * 2]);     // body

			barObj->AddComponent(new CTransform);
			barObj->AddComponent(new CSpriteRenderer);
			barObj->AddComponent(new Note(m_noteSpeed, m_dir));

			barObj->SpriteRenderer()->SetSprite(sprite);
			barObj->SpriteRenderer()->SetMesh(FIND(AMesh, L"RectMesh"));

			barObj->GetTransform()->SetIndependentScale(true);
			barObj->GetTransform()->SetRelativeScale(Vec3(35.f, 100.f, 1.f));

			GetOwner()->AddChild(barObj);

			float localTime = startTime * 0.001f + timeOffset;
			float yOffset = m_noteSpeed * localTime + 200.f;

			barObj->GetTransform()->SetRelativePosition(
				Vec3(0.f, -yOffset - spriteHeight * j, 0.f)
			);
		}
	}
}
