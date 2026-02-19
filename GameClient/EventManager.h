#pragma once
#include "CScript.h"

struct CameraEventInfo
{
	float timing;
	int value;
};

class CPlayerController;
class COpponentController;
class CCamera;

class EventManager : public CScript
{
public:
	EventManager();
	virtual ~EventManager();

	virtual void Begin() override;
	virtual void Tick() override;

public:
	void SetPlayer(Ptr<CPlayerController> player) { m_player = player; }
	void SetOpponent(Ptr<COpponentController> opponent) { m_opponent = opponent; }
	void SetCam(Ptr<CCamera> cam) { m_cam = cam; }

	void Start() { m_isStarted = true; }

public:
	void AddCameraEvent(CameraEventInfo info) { m_camEventVec.push_back(info); }
	void AddAnimEvent(float timing) { m_animEventVec.push_back(timing); }

private:
	vector<CameraEventInfo> m_camEventVec;
	vector<float> m_animEventVec;		// animaiton은 HEY!!로 고정

	int m_camEventIdx;
	int m_animEventIdx;

	bool m_isStarted;

	float m_elapsed;

	Ptr<CPlayerController> m_player;
	Ptr<COpponentController> m_opponent;
	Ptr <CCamera> m_cam;

	Vec3 m_playerCamPos;
	Vec3 m_opponentCamPos;
};

