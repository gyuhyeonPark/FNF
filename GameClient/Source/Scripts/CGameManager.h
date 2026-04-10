#pragma once
#include "CScript.h"
#include "CNoteReceptor.h"

class CCharacterController;

class CGameManager : public CScript
{
public:
	CGameManager();
	CGameManager(const CGameManager& _Origin);
	virtual ~CGameManager();

	virtual void Begin() override;

	virtual void Tick() override;
	
	void SetPlayerController(Ptr<CCharacterController> controller);
	void SetOpponentController(Ptr<CCharacterController> controller);

	void MusicStart();

	void SetDifficulty(DIFFICULTIES diff) { m_diff = diff; }
	void SetCamComp(Ptr<CCamera> cam) { m_cam = cam.Get(); }
	void SetOpponent(Ptr<GameObject> obj);
	void SetNoteReceptor(int _idx, Ptr<CNoteReceptor> _ncr)
	{
		noteReceptors[_idx] = _ncr;
	}

	Ptr<CNoteReceptor>* GetNoteReceptors() { return noteReceptors; }

	CLONE(CGameManager);
	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	void KeyTapUpdate();
	void KeyPressedUpdate();
	void KeyReleasedUpdate();

private:
	Ptr<CNoteReceptor> noteReceptors[NOTE_SIZE];		// 0 ~ 3 : Player, 4 ~ 7 : Opponent

/*	Vec3 m_opponentNRPos = Vec3(-400.f, 200.f, 2.f);
	Vec3 m_playerNRPos = Vec3(100.f, 200.f, 2.f);*/

	DIFFICULTIES m_diff;
	CCamera* m_cam;

	Ptr<CCharacterController> m_playerController;
	Ptr<CCharacterController> m_opponentController;

	Ptr<CAtlasAnimator> m_gfAnim;
};

