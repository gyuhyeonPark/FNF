#pragma once
#include "CScript.h"

struct CameraEventInfo
{
	float timing;
	int value;
};

struct LightEventInfo
{
	float start;
	float end;
	int value;
};

class CPlayerController;
class COpponentController;
class CCamera;
class CDotween;

class CEventManager : public CScript
{
public:
	CEventManager();
	//CEventManager(const CEventManager& _Origin);
	
	virtual ~CEventManager();

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
	void AddLightEvent(LightEventInfo info) { m_lightEventVec.push_back(info); }

	CLONE(CEventManager);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	vector<CameraEventInfo> m_camEventVec;
	vector<float> m_animEventVec;		// animaiton은 HEY!!로 고정
	vector<LightEventInfo> m_lightEventVec;

	int m_camEventIdx;
	int m_animEventIdx;
	int m_lightEventIdx;

	bool m_isStarted;

	float m_elapsed;
	float m_zoomElapsed;

	bool m_isZoomStarted;

	Ptr<CPlayerController> m_player;
	Ptr<COpponentController> m_opponent;
	Ptr<CCamera> m_cam;
	Ptr<CDotween> m_camDotween;

	Ptr<CLight2D> m_spotLight[2];
	Ptr<CLight2D> m_directionalLight;

	char m_lightFlag;

	Vec3 m_playerCamPos;
	Vec3 m_opponentCamPos;
};

