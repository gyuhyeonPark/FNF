#pragma once
class PathManager : public SingletonClass<PathManager>
{
	SINGLE(PathManager);
private:
	// 255 : 애초에 폴더 문자열 길이를 windows가 제한하고 있다.
	wchar_t m_contentPath[255];
public:
	~PathManager();

	const wchar_t* GetContentsPath() { return m_contentPath; }

private:
	void Init();
};

