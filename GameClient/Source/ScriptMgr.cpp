#include "pch.h"
#include "ScriptMgr.h"

/*#include "Scripts/CAtlasAnimator.h"*/
#include "Scripts/CCameraMove.h"
#include "Scripts/CCharacterController.h"
#include "Scripts/CComboImage.h"
#include "Scripts/CComboManager.h"
#include "Scripts/CDotween.h"
#include "Scripts/CDotweenTimer.h"
#include "Scripts/CEventManager.h"
#include "Scripts/CGameManager.h"
#include "Scripts/CMissileScript.h"
#include "Scripts/CNote.h"
#include "Scripts/CNoteReceptor.h"
#include "Scripts/COpponentController.h"
#include "Scripts/COpponentNoteReceptor.h"
#include "Scripts/CPlayerController.h"
#include "Scripts/CPlayerNoteReceptor.h"
#include "Scripts/CPlayerScript.h"
#include "Scripts/CStartingPistol.h"
#include "Scripts/Easing.h"

void ScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
/*	_vec.push_back(L"CAtlasAnimator");*/
	_vec.push_back(L"CCameraMove");
	_vec.push_back(L"CCharacterController");
	_vec.push_back(L"CComboImage");
	_vec.push_back(L"CComboManager");
	_vec.push_back(L"CDotween");
	_vec.push_back(L"CDotweenTimer");
	_vec.push_back(L"CEventManager");
	_vec.push_back(L"CGameManager");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CNote");
	_vec.push_back(L"CNoteReceptor");
	_vec.push_back(L"COpponentController");
	_vec.push_back(L"COpponentNoteReceptor");
	_vec.push_back(L"CPlayerController");
	_vec.push_back(L"CPlayerNoteReceptor");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CStartingPistol");
	_vec.push_back(L"Easing");
}

CScript * ScriptMgr::GetScript(const wstring& _strScriptName)
{

	if (L"CCameraMove" == _strScriptName)
		return new CCameraMove;

	if (L"CComboImage" == _strScriptName)
		return new CComboImage;

	if (L"CDotween" == _strScriptName)
		return new CDotween;

	if (L"CEventManager" == _strScriptName)
		return new CEventManager;
	if (L"CGameManager" == _strScriptName)
		return new CGameManager;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CNote" == _strScriptName)
		return new CNote;

	if (L"COpponentController" == _strScriptName)
		return new COpponentController;
	if (L"COpponentNoteReceptor" == _strScriptName)
		return new COpponentNoteReceptor;
	if (L"CPlayerController" == _strScriptName)
		return new CPlayerController;
	if (L"CPlayerNoteReceptor" == _strScriptName)
		return new CPlayerNoteReceptor;

	if (L"CStartingPistol" == _strScriptName)
		return new CStartingPistol;

	return nullptr;
}

CScript * ScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{

	case (UINT)SCRIPT_TYPE::CAMERAMOVE:
		return new CCameraMove;
		break;

	case (UINT)SCRIPT_TYPE::COMBOIMAGE:
		return new CComboImage;
		break;

	case (UINT)SCRIPT_TYPE::DOTWEEN:
		return new CDotween;
		break;

	case (UINT)SCRIPT_TYPE::EVENTMANAGER:
		return new CEventManager;
		break;
	case (UINT)SCRIPT_TYPE::GAMEMANAGER:
		return new CGameManager;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::NOTE:
		return new CNote;
		break;

	case (UINT)SCRIPT_TYPE::OPPONENTCONTROLLER:
		return new COpponentController;
		break;
	case (UINT)SCRIPT_TYPE::OPPONENTNOTERECEPTOR:
		return new COpponentNoteReceptor;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERCONTROLLER:
		return new CPlayerController;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERNOTERECEPTOR:
		return new CPlayerNoteReceptor;
		break;

	case (UINT)SCRIPT_TYPE::STARTINGPISTOL:
		return new CStartingPistol;
		break;
	}
	return nullptr;
}

const wchar_t * ScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
/*	case SCRIPT_TYPE::ATLASANIMATOR:
		return L"CAtlasAnimator";
		break;*/

	case SCRIPT_TYPE::CAMERAMOVE:
		return L"CCameraMove";
		break;

	case SCRIPT_TYPE::CHARACTERCONTROLLER:
		return L"CCharacterController";
		break;

	case SCRIPT_TYPE::COMBOIMAGE:
		return L"CComboImage";
		break;

	case SCRIPT_TYPE::DOTWEEN:
		return L"CDotween";
		break;

	case SCRIPT_TYPE::DOTWEENTIMER:
		return L"CDotweenTimer";
		break;

	case SCRIPT_TYPE::EVENTMANAGER:
		return L"CEventManager";
		break;

	case SCRIPT_TYPE::GAMEMANAGER:
		return L"CGameManager";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::NOTE:
		return L"CNote";
		break;

	case SCRIPT_TYPE::NOTERECEPTOR:
		return L"CNoteReceptor";
		break;

	case SCRIPT_TYPE::OPPONENTCONTROLLER:
		return L"COpponentController";
		break;

	case SCRIPT_TYPE::OPPONENTNOTERECEPTOR:
		return L"COpponentNoteReceptor";
		break;

	case SCRIPT_TYPE::PLAYERCONTROLLER:
		return L"CPlayerController";
		break;

	case SCRIPT_TYPE::PLAYERNOTERECEPTOR:
		return L"CPlayerNoteReceptor";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::STARTINGPISTOL:
		return L"CStartingPistol";
		break;

	}
	return nullptr;
}