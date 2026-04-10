#pragma once
#include "CRenderComponent.h"
#include "AFlipbook.h"

class CFlipbookRenderer : public CRenderComponent
{
public:
	CFlipbookRenderer();
	virtual ~CFlipbookRenderer();

	virtual void FinalTick() override;
	virtual void Render() override;
	virtual void CreateMaterial() override;

public:
	void SetFlipbook(int _idx, Ptr<AFlipbook> _flipbook)
	{
		if (m_vecFlipbook.size() <= _idx)
			m_vecFlipbook.resize(_idx + 1);
		m_vecFlipbook[_idx] = _flipbook;
	}

	void AddFlipbook(Ptr<AFlipbook> _flipbook) { m_vecFlipbook.push_back(_flipbook); }

	void Play(int _FlipbookIdx, float _FPS, int _RepeatCount)
	{
		m_CurFlipbook = _FlipbookIdx;
		m_RepeatCount = _RepeatCount;
		m_FPS = _FPS;
		m_AccTime = 0.f;
	}

private:
	bool CheckFinish();

public:
	CLONE(CFlipbookRenderer);

	virtual void SaveToSceneFile(FILE* _File) override;
	virtual void LoadFromSceneFile(FILE* _File) override;

private:
	vector<Ptr<AFlipbook>>  m_vecFlipbook;

	int                     m_CurFlipbook;
	int                     m_CurSprite;

	int                     m_RepeatCount;  // -1 : 반복재생, 1 이상이면 재생 횟수
	bool                    m_Finish;
	float                   m_FPS;
	float                   m_AccTime;
};

