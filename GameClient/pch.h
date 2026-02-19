#pragma once

// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include "resource.h"

#define IMGUI_DEFINE_MATH_OPERATORS

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <memory>
#include <tchar.h>

// D3D
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "SimpleMath.h"
#include "SimpleMath.inl"
#include <DirectXPackedVector.h>
// 프로젝트 추가 디렉터리 설정을 Libraries\Include 까지 해줬기 때문에 아래와 같이 경로 설정 가능.
#include <DirectXTex\\DirectXTex.h>
#include <DirectXTex\\DirectXTex.inl>
using namespace DirectX;
using namespace DirectX::PackedVector;

// 라이브러리?
// windows.h 의 경우 기본적으로 헤더파일만 include해도 자동으로 내부 라이브러리와 링킹이 된다.
// 하지만 GPU를 제어하는 DirectX 라이브러리의 경우 일반적으로 쓰이지 않기 때문에...
// 전방선언(헤더파일 include)뿐만 아니라 명시적으로 아래와 같은 형태로 구현부와의 링킹을 해줘야 한다.
// 즉, 라이브러리는 cpp다!
// 그러면 왜 .cpp가 아니고 .lib일까?
// -> 어느정도 컴파일 된 상태의 파일이기 때문. 구현부를 공개하고 싶지 않기 때문에!
// ------------------------------------------------------------------------------------------------
// " " 와 < > 의 차이?
// " " : 사용자가 프로젝트를 위해 직접 제작한 헤더파일. 즉, 상대 경로를 의미한다.
// < > : visual studio 설치 시 기본으로 외부 경로에 설치된 헤더파일. 즉, 절대 경로를 의미한다.
// ------------------------------------------------------------------------------------------------
// DLL (Dynamic Linked Library)
// 주의점 : DLL에서 동적할당한 객체를 제공할 경우 꼭 delete해주는 기능의 함수를 제공하기.

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid")

#ifdef _DEBUG		// 프로젝트 설정 중, c/c++ 전처리기 정의에 따른 매크로 불러오기 가능.
#pragma comment (lib, "DirectXTex/DirectXTex_debug.lib")
#else
#pragma comment (lib, "DirectXTex/DirectXTex.lib")
#endif

// ComPtr
#include <wrl.h>
using namespace Microsoft::WRL;

// STL
#include <vector>
#include <list>
#include <set>
#include <map>
#include <string>
#include <filesystem>
#include <functional>

using namespace std;

// XML Parser
#include "tinyxml2.h" 
using namespace tinyxml2;

// Json Parser
#include "rapidjson\\document.h"
#include "rapidjson\\writer.h"
#include "rapidjson\\stringbuffer.h"
#include "rapidjson\\filereadstream.h"

// FMOD
#include "FMOD\\fmod.h"
#include "FMOD\\fmod.hpp"
#include "FMOD\\fmod_codec.h"
#include "FMOD\\fmod_common.h"
#include "FMOD\\fmod_dsp.h"
#include "FMOD\\fmod_dsp_effects.h"
#include "FMOD\\fmod_errors.h"
#include "FMOD\\fmod_output.h"
using namespace FMOD;

#pragma comment(lib, "FMOD/fmod_vc.lib")

#include "Macros.h"
#include "Types.h"
#include "Values.h"
#include "enum.h"
#include "struct.h"
#include "SingletonClass.h"
#include "func.h"

// custom smart poiner
#include "Ptr.h"

// Engine
#include "Engine.h"
#include "Graphics.h"
#include "SoundManager.h"
#include "Parser.h"
#include "PathManager.h"
#include "TimeManager.h"
#include "KeyManager.h"




