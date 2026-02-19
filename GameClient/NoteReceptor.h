#pragma once
#include "CScript.h"
/// <summary>
/// 노트 입력을 받는 클래스
/// GameManager에서 배열로 방향키 관리
/// </summary>

enum class DIFFICULTIES
{
	EASY,
	NORMAL,
	HARD,

	END,
}; 

enum class ARROW_DIR
{
	LEFT,
	DOWN,
	UP,
	RIGHT,

	END,
};

const char* defaultArrowName[4]
{
	"staticLeft",
	"staticDown",
	"staticUp",
	"staticRight",
};

// Note 타이밍 상관 없이 항상 적용되는 Animation
const char* tapArrowName[4]
{
	"pressLeft",
	"pressDown",
	"pressUp",
	"pressRight",
};

// 노트 입력 성공 시 적용되는 Animation
const char* confirmPressedName[4]
{
	"confirmHoldLeft",
	"confirmHoldDown",
	"confirmHoldUp",
	"confirmHoldRight",
};

const char* confirmTapName[4]
{
	"confirmLeft",
	"confirmDown",
	"confirmUp",
	"confirmRight",
};

class CharacterController;
class CAtlasAnimator;

class NoteReceptor : public CScript
{
public:
	NoteReceptor();
	virtual ~NoteReceptor();

	/// <summary>
	/// Tick에서 해줘야 할 일
	/// 1. 가만히 있을 경우 노트가 허용범위 초과 시 miss 이벤트 발생
	/// 2. m_tapTimingIdx++
	/// </summary>
	virtual void Begin() override;
	virtual void Tick() = 0;

	virtual void ActivateTapEvent() = 0;
	virtual void ActivatePressedEvent() = 0;
	virtual void ActivateReleasedEvent() = 0;

	// TEMP
	virtual void SetOpponentObj(Ptr<GameObject> obj) { m_opponentObj = obj; }

	void SetDifficulties(DIFFICULTIES d) { m_difficulty = d; }
	void SetDir(ARROW_DIR dir) { m_dir = dir; }
	void SetCharacterController(Ptr<CharacterController> controller) { m_characterController = controller; }
	void SetAnimator(Ptr<CAtlasAnimator> anim) { m_anim = anim; }

	void MusicStart() { m_isMusicStart = true; }

protected:
	vector<float> tapTimings[UINT(DIFFICULTIES::END)];
	vector<pair<float, float>> pressTimings[UINT(DIFFICULTIES::END)];	// timing, duration
	float m_elapsed;
	int m_tapTimingIdx;
	int m_pressedTimingIdx;

	bool m_isTapped;
	bool m_isMusicStart;

	DIFFICULTIES m_difficulty;
	ARROW_DIR m_dir;
	Ptr<CharacterController> m_characterController;
	Ptr<CAtlasAnimator> m_anim;

	// TEMP
	Ptr<GameObject> m_opponentObj;

	float m_noteSpeed;

	friend class Parser;
	friend class GameManager;
};

