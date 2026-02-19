#include "pch.h"
#include "PathManager.h"

PathManager::PathManager()
	: m_contentPath{}
{
	Init();
}

PathManager::~PathManager()
{
}

void PathManager::Init()
{
	// ContentPath 경로 찾기
	// 실행파일이 있는 Bin 폴더 경로를 찾아낸다.
	// 디버깅 모드에서도 똑같이 동작하게 하기 위해서, 
	// 프로젝트 구성설정, 디버깅 탭에 작업 디렉터리를 실행파일 경로로 설정해준다.
	GetCurrentDirectory(255, m_contentPath);
	//SetWindowText(Engine::GetInst()->GetMainWndHwnd(), m_ContentPath);

	// .exe 폴더 경로 찾고, 뒤에서 부터 탐색
	// -> \\ 찾을 시 제거 후 Content 경로로 수정.
	int Len = wcslen(m_contentPath);

	for (int i = Len - 1; 0 <= i; --i)
	{
		if ('\\' == m_contentPath[i])
		{
			m_contentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_contentPath, L"\\Contents\\");
}


