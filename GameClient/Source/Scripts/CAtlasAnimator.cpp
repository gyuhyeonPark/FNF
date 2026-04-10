#include "pch.h"
#include "CAtlasAnimator.h"
#include "GameObject.h"
#include "AssetManager.h"
#include "SongManager.h"

CAtlasAnimator::CAtlasAnimator()
	: Component(COMPONENT_TYPE::ATLASANIMATOR), m_animMap{},
	m_currentAnimKey(""), m_animTime(0.f), m_curFrame(0.f),
	m_isLoop(false), m_isAnimEnd(false), m_isVolatile(false),
	m_atlasTexture(nullptr), m_animEndEvent(nullptr)
{
}

CAtlasAnimator::CAtlasAnimator(const CAtlasAnimator& _Origin)
	: Component(_Origin), m_animMap(_Origin.m_animMap),
	m_currentAnimKey(_Origin.m_currentAnimKey), m_animTime(0.f), m_curFrame(0.f),
	m_isLoop(_Origin.m_isLoop), m_isAnimEnd(false), m_isVolatile(_Origin.m_isVolatile),
	m_atlasTexture(_Origin.m_atlasTexture), m_animEndEvent(_Origin.m_animEndEvent)
{
	
}

CAtlasAnimator::~CAtlasAnimator()
{
}

void CAtlasAnimator::Begin()
{
	// Volatile이건 아니건...
	if (GetOwner()->GetMeshRenderer()->GetMaterial() != nullptr)
	{
		MeshRenderer()->GetMaterial()->SetScalar(INT_0, 1);
	}
	m_basePos = Transform()->GetRelativePosition();
}

void CAtlasAnimator::FinalTick()
{
	// Texture가 Animation으로써 사용될 때 바인딩 -> 화면에 출력해준다
	UpdateAnimation(DT);
}

void CAtlasAnimator::UpdateAnimation(float dt)
{
	if (!m_isAnimEnd)
		m_animTime += DT;

	// 프레임 갱신 시간 (Idle Animation 기준) : bpm / 100 / 14 (IdleFrameSize)
	// 한 박(60 / bpm)에 14 프레임이 돌아야 한다.

	float bpm = BPM;
	if (bpm >= 150)
		bpm /= 2;

	const float frameDuration = 60.f / bpm / IDLE_FRAME_SIZE;

	if (m_animTime >= frameDuration)
	{
		m_animTime -= frameDuration;
		if (!m_isLoop && m_curFrame >= m_animMap[m_currentAnimKey].frames.size() - 1)
		{
			// 애니메이션 끝
			m_isAnimEnd = true;
			// 이펙트와 같은 휘발성이라면? -> material 조작해서 discard 시키기?
			if (m_isVolatile)
				MeshRenderer()->GetMaterial()->SetScalar(INT_0, 1);
		}
		else
			m_curFrame = (m_curFrame + 1) % m_animMap[m_currentAnimKey].frames.size();

		if (m_animEndEvent)
		{
			m_animEndEvent();
			m_animEndEvent = nullptr;
		}
	}

	// atlas uv mapping
	const AtlasFrame& f = m_animMap[m_currentAnimKey].frames[m_curFrame];

	float scaleX = Transform()->GetRelativeScale().x;
	float scaleY = Transform()->GetRelativeScale().y;

	const AtlasFrame& frame = m_animMap[m_currentAnimKey].frames[m_curFrame];

/*	// base position (애니메이션 적용 전 기준 위치)
	Vec3 basePos = m_basePos;

	// frame offset
	Vec2 animOffset(
		frame.frameX / (scaleX * 0.2f),
		frame.frameY / (scaleY * 0.2f)
	);

	// 최종 위치
	Transform()->SetRelativePosition(
		Vec3(basePos.x + animOffset.x,
			basePos.y + animOffset.y,
			basePos.z));*/

	// uv lerp
	Vec4 uv = { f.u0, f.u1, f.v0, f.v1 };

	Transform()->SetUV(uv);
	Transform()->SetUVRot(f.rotated);
}

void CAtlasAnimator::LoadMapInfo(Ptr<ATexture> tex)
{
	assert(m_atlasTexture == nullptr);
	assert(m_currentAnimKey == "");
	m_atlasTexture = tex.Get();

	m_atlasTexture->m_fileName = m_atlasTexture->GetRelativePath();

	size_t dotPos = m_atlasTexture->m_fileName.find_last_of('.');
	if (dotPos != std::string::npos)
	{
		m_atlasTexture->m_fileName = m_atlasTexture->m_fileName.substr(0, dotPos);
	}
	m_atlasTexture->m_fileName += L".xml";

	wstring contentsPath = PathManager::GetInstance()->GetContentsPath()
		+ m_atlasTexture->m_fileName;

	int size = WideCharToMultiByte(
			CP_UTF8, 0,
			contentsPath.c_str(), -1,
			nullptr, 0,
			nullptr, nullptr
		);

	std::string xmlPath(size - 1, 0);

	WideCharToMultiByte(
		CP_UTF8, 0,
		contentsPath.c_str(), -1,
		&xmlPath[0], size,
		nullptr, nullptr
	);

	// 만약 Atlas 이미지라면, 해당 내용을 담아준다.
	PARSER->LoadTextureAtlas(xmlPath.c_str(), m_atlasTexture->m_width, m_atlasTexture->m_height, m_animMap);
	assert(m_animMap.size() != 0);

	m_currentAnimKey = m_animMap.begin()->first;
}

void CAtlasAnimator::Play(const string& key, bool isLoop)
{
	// volatile이었을 경우 다시 띄워주기
	if (m_isVolatile)
		MeshRenderer()->GetMaterial()->SetScalar(INT_0, 0);

	m_currentAnimKey = key; 
	m_curFrame = 0;
	m_isLoop = isLoop;
	m_isAnimEnd = false;
}

void CAtlasAnimator::Discard()
{
	assert(m_isVolatile, "Volatile이 꺼져있는 애니메이터를 Discard할 수 없습니다.");
	MeshRenderer()->GetMaterial()->SetScalar(INT_0, 1);
	m_curFrame = 0;
	m_isAnimEnd = true;
}

void CAtlasAnimator::SaveToSceneFile(FILE* _File)
{
	fwrite(&m_atlasTexture, sizeof(ATexture), 1, _File);
	fwrite(&m_currentAnimKey, sizeof(string), 1, _File);
	fwrite(&m_animTime, sizeof(float), 1, _File);
	fwrite(&m_curFrame, sizeof(int), 1, _File);
	fwrite(&m_isLoop, sizeof(bool), 1, _File);
	fwrite(&m_isAnimEnd, sizeof(bool), 1, _File);
	fwrite(&m_isVolatile, sizeof(bool), 1, _File);

	size_t animationCnt = m_animMap.size();
	fwrite(&animationCnt, sizeof(size_t), 1, _File);

	for (auto& each : m_animMap)
	{
		size_t len = each.first.size();
		fwrite(&len, sizeof(size_t), 1, _File);
		fwrite(each.first.data(), sizeof(char), len, _File);

		each.second.SaveAtlasAnimation(_File);
	}
}

void CAtlasAnimator::LoadFromSceneFile(FILE* _File)
{
	fread(&m_atlasTexture, sizeof(ATexture), 1, _File);
	fread(&m_currentAnimKey, sizeof(string), 1, _File);
	fread(&m_animTime, sizeof(float), 1, _File);
	fread(&m_curFrame, sizeof(int), 1, _File);
	fread(&m_isLoop, sizeof(bool), 1, _File);
	fread(&m_isAnimEnd, sizeof(bool), 1, _File);
	fread(&m_isVolatile, sizeof(bool), 1, _File);

	size_t animationCnt = 0;
	fread(&animationCnt, sizeof(size_t), 1, _File);

	m_animMap.clear();

	for (size_t i = 0; i < animationCnt; ++i)
	{
		// key 읽기
		size_t len;
		fread(&len, sizeof(size_t), 1, _File);

		std::string key;
		key.resize(len);
		fread(key.data(), sizeof(char), len, _File);

		// value 읽기
		AtlasAnimation anim;
		anim.LoadAtlasAnimation(_File);

		// map에 삽입
		m_animMap.insert({ key, anim });
	}
}
