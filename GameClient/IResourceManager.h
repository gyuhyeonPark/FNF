#pragma once

enum STAGENUM
{
	SHARED = 0,
	BOPEEBO,


	STAGEEND,
};

/// <summary>
/// binding with STAGENUM
///	stageFolderPath[STAGENUM] => foldername
/// </summary>
static string StageFolderPath[] =
{
	"shared",
	"bopeebo",
};

/// <summary>
/// Stage에 따라 그에 맞는 리소스를 생성하는 클래스가 상속받는다.
/// ex) Sound 등
/// </summary>
class IResourceManager
{
public:
	virtual void Init() abstract;
	virtual void CreateResources(STAGENUM stageNumber) abstract;
	virtual ~IResourceManager() {};
};

