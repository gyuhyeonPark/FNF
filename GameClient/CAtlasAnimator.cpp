#include "pch.h"
#include "CAtlasAnimator.h"
#include "GameObject.h"

CAtlasAnimator::CAtlasAnimator()
	: m_animTime(0.f), m_curFrame(0.f), m_isLoop(false), m_isAnimEnd(true), m_isVolatile(false)
{
}

CAtlasAnimator::~CAtlasAnimator()
{
}

void CAtlasAnimator::Begin()
{
	// Volatile이건 아니건...
	if (GetOwner()->GetMeshRenderer()->GetMtrl() != nullptr)
		MeshRenderer()->GetMtrl()->SetScalar(INT_0, 1);
}

void CAtlasAnimator::Tick()
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
	const float frameDuration = 60.f / BOPEEBO_BPM / IDLE_FRAME_SIZE;

	if (m_animTime >= frameDuration)
	{
		m_animTime -= frameDuration;
		if (!m_isLoop && m_curFrame >= m_animMap[m_currentAnimKey].frames.size() - 1)
		{
			// 애니메이션 끝
			m_isAnimEnd = true;
			// 이펙트와 같은 휘발성이라면? -> material 조작해서 discard 시키기?
			if (m_isVolatile)
				MeshRenderer()->GetMtrl()->SetScalar(INT_0, 1);
		}
		else
			m_curFrame = (m_curFrame + 1) % m_animMap[m_currentAnimKey].frames.size();
	}

	// atlas uv mapping
	Ptr<CTransform> transform = GetOwner()->GetTransform();
	const AtlasFrame& f = m_animMap[m_currentAnimKey].frames[m_curFrame];

	// uv lerp
	Vec4 uv = { f.u0, f.u1, f.v0, f.v1 };

	transform->SetUV(uv);
	transform->SetUVRot(f.rotated);
}

void CAtlasAnimator::LoadMapInfo(Ptr<ATexture> tex)
{
	m_atlasTexture = tex;

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
	m_currentAnimKey = m_animMap.begin()->first;
}

void CAtlasAnimator::Play(const string& key, bool isLoop)
{
	// volatile이었을 경우 다시 띄워주기
	if (m_isVolatile)
		MeshRenderer()->GetMtrl()->SetScalar(INT_0, 0);

	m_currentAnimKey = key; 
	m_curFrame = 0;
	m_isLoop = isLoop;
	m_isAnimEnd = false;
}
