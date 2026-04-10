#pragma once
/// <summary>
/// AudiowaveUI에서 그려줄 피킹 가능한 객체들의 인터페이스 클래스.
/// </summary>
class IPickable
{
public:
	virtual void UpdatePos() = 0;		// 노드들의 위치 갱신
	virtual void DetectPicking() = 0;
	virtual void Tick() = 0;
};

