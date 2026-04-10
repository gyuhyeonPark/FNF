#pragma once
#include "CScript.h"
#include "ATexture.h"

class CAtlasAnimator : public Component
{
public:
	CAtlasAnimator();
	CAtlasAnimator(const CAtlasAnimator& _Origin);
	~CAtlasAnimator();

public:
	virtual void Begin() override;
	virtual void FinalTick() override;

private:
	void UpdateAnimation(float dt);

public:
	void LoadMapInfo(Ptr<ATexture> tex);
	std::unordered_map<string, AtlasAnimation>& GetAnimMap() { return m_animMap; }
	void Play(const string& key, bool isLoop = false);

	void SetAsVolatile() 
	{
		m_isVolatile = true;
		MeshRenderer()->GetMaterial()->SetScalar(INT_0, 1);
	}

	void Discard();

	CLONE(CAtlasAnimator);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

	void SetAnimEndEvent(std::function<void()> func) { m_animEndEvent = func; }

private:
	ATexture* m_atlasTexture;
	string m_currentAnimKey;
	std::unordered_map<string, AtlasAnimation> m_animMap;
	float m_animTime = 0.0f;
	int m_curFrame = 0;
	bool m_isLoop;
	bool m_isAnimEnd;
	bool m_isVolatile;
	Vec3 m_basePos;

	std::function<void()> m_animEndEvent;
};

