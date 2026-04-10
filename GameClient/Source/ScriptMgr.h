#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
/*	ATLASANIMATOR,*/
	CAMERAMOVE,
	CHARACTERCONTROLLER,
	COMBOIMAGE,
	DOTWEEN,
	DOTWEENTIMER,
	EVENTMANAGER,
	GAMEMANAGER,
	MISSILESCRIPT,
	NOTE,
	NOTERECEPTOR,
	OPPONENTCONTROLLER,
	OPPONENTNOTERECEPTOR,
	PLAYERCONTROLLER,
	PLAYERNOTERECEPTOR,
	PLAYERSCRIPT,
	STARTINGPISTOL,
	BEATCAMERA,
};

using namespace std;

class CScript;

class ScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
