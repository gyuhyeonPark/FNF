#pragma once
#include "CScript.h"

enum class ComboTagIdx
{
	IDX_SHIT,
	IDX_BAD,
	IDX_GOOD,
	IDX_SICK,
};

class ComboManager : public CScript
{
public:
	ComboManager();
	virtual ~ComboManager();

public:
	virtual void Init();
	virtual void Tick();

public:
	void AddCombo(ComboTagIdx _judge);
	void ResetCombo();

private:
	int m_currentCombo;
	vector<int> m_comboNumVec;
	vector<Ptr<GameObject>> m_tagSprites;
	vector<Ptr<GameObject>> m_numSprites;

	Ptr<GameObject> m_comboSprite;

	Vec3 m_destination;
	Vec3 m_startPosition;

	ComboTagIdx m_currentTag;

private:
	float m_elapsed;
	float m_duration;
	bool m_isPopStart;
	const float gravity;
	float m_velocity;
};

