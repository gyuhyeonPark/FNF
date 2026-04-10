#pragma once
#include "SingletonClass.h"
#include "AScene.h"
#include "Layer.h"
#include "CCollider2D.h"


union COL_ID
{
	struct
	{
		UINT	leftID;
		UINT	rightID;

	};
	ULONGLONG	 ID;		// 8바이트를 left, right로 나눔. left가 하위비트(오른쪽)
};

class CollisionManager : public SingletonClass<CollisionManager>
{
	SINGLE(CollisionManager);
public:
	virtual ~CollisionManager();

	void Progress(Ptr<AScene> scene);

private:
	void ColisionBtwLayer(Layer* left, Layer* right);

	bool IsCollision(Ptr<CCollider2D> leftColl, Ptr<CCollider2D> rightColl);

private:
	map<ULONGLONG, bool> m_mapColID;
};

