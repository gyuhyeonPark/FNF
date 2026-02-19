#pragma once
#include "CScript.h"
#include "NoteReceptor.h"

class CharacterController;

class GameManager : public CScript
{
public:
	GameManager();
	virtual ~GameManager();
	void Init(DIFFICULTIES diff, Ptr<CCamera> cam);

	virtual void Tick() override;
	
	void SetPlayerController(Ptr<CharacterController> controller);
	void SetOpponentController(Ptr<CharacterController> controller);

	void MusicStart();

	void SetOpponent(Ptr<GameObject> obj);

private:
	void KeyTapUpdate();
	void KeyPressedUpdate();
	void KeyReleasedUpdate();

private:
	Ptr<NoteReceptor> noteReceptors[NOTE_SIZE];		// 0 ~ 3 : Player, 4 ~ 7 : Opponent

	Vec3 m_opponentNRPos = Vec3(-400.f, 200.f, 2.f);
	Vec3 m_playerNRPos = Vec3(100.f, 200.f, 2.f);
};

