#pragma once
#include "CScript.h"

enum class JudgeMent
{
	E_MISS,
	E_SHIT,
	E_BAD,
	E_GOOD,
	E_SICK,
};


/// <summary>
/// 함수 호출 시 현재 콤보에 맞는 스프라이트를 생성,
/// 해당 오브젝트에 CComboImage 컴포넌트를 장착해 생성한다.
/// </summary>
class CComboManager : public SingletonClass<CComboManager>
{
	SINGLE(CComboManager);
public:
	virtual ~CComboManager();

public:
	void AddCombo(JudgeMent _val) { m_currentCombo++; CreateComboImage(_val); }
	void ResetCombo(JudgeMent _val) { m_currentCombo = 0; CreateComboImage(_val); }

private:
	void CreateComboImage(JudgeMent _val);

private:
	UINT m_currentCombo;
	const Vec3 m_startPos;
};

                     