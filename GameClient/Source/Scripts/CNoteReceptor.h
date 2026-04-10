#pragma once
#include "CScript.h"
#include "SongManager.h"

/// <summary>
/// 노트 입력을 받는 클래스
/// GameManager에서 배열로 방향키 관리
/// </summary>

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

// CNote 타이밍 상관 없이 항상 적용되는 Animation
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

class CCharacterController;
class CAtlasAnimator;
class CNote;

struct LongNote
{
	CNote* body;
	CNote* tail;
};

class CNoteReceptor : public CScript
{
public:
	CNoteReceptor(SCRIPT_TYPE _type);
	CNoteReceptor(const CNoteReceptor& _Origin);
	virtual ~CNoteReceptor();

	/// <summary>
	/// Tick에서 해줘야 할 일
	/// 1. 가만히 있을 경우 노트가 허용범위 초과 시 miss 이벤트 발생
	/// 2. m_tapTimingIdx++
	/// </summary>
	virtual void Begin() override;
	virtual void Tick();

	virtual void ActivateTapEvent() = 0;
	virtual void ActivatePressedEvent() = 0;
	virtual void ActivateReleasedEvent() = 0;

	// TEMP
	virtual void SetOpponentObj(Ptr<GameObject> obj) { m_opponentObj = obj; }

	void SetDir(ARROW_DIR dir) { m_dir = dir; }
	void SetCharacterController(Ptr<CCharacterController> controller) { m_characterController = controller; }
	void SetAnimator(Ptr<CAtlasAnimator> anim) { m_anim = anim; }

	void MusicStart();

	virtual CScript* Clone() = 0;

	const vector<float>& GetTapTimings() { return tapTimings; }
	const vector<pair<float, float>>& GetPressTimings() { return pressTimings; }

	void SetNrNum(int _id) { m_nrNum = _id; }

protected:
	void DestroyTapNote();
	void DestroyPressNote();

protected:
	vector<float> tapTimings;
	vector<pair<float, float>> pressTimings;	// timing, duration
	
	// Timings와 동일한 size의 Note vector
	vector<CNote*> m_tapNoteVec;
	vector<LongNote> m_pressNoteVec;
	
	queue<CNote*> m_tapNoteQueue;
	queue<LongNote> m_pressNoteQueue;

	float m_elapsed;
	int m_tapTimingIdx;
	int m_pressedTimingIdx;

	bool m_isTapped;
	bool m_isMusicStart;

	ARROW_DIR m_dir;
	Ptr<CCharacterController> m_characterController;
	Ptr<CAtlasAnimator> m_anim;

	// TEMP
	Ptr<GameObject> m_opponentObj;

	float m_noteSpeed;

private:
	float m_appearElapsed;		// 노트 등장 elapsed
	
	int m_nrNum;				// 자신의 인덱스 0 ~ 8

	int m_tapNoteIdx;
	int m_pressNoteIdx;

	friend class Parser;
	friend class CGameManager;
};

