#pragma once
#include "CScript.h"
class BeatCamera : public CScript
{
public:
	BeatCamera();
	BeatCamera(const BeatCamera& _origin);
	virtual ~BeatCamera();

	CLONE(BeatCamera);


	virtual void SaveToSceneFile(FILE* _File) {};
	virtual void LoadFromSceneFile(FILE* _File) {};

public:
	virtual void Tick() override;

private:
	float m_elapsed;
};

