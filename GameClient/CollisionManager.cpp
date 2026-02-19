#include "pch.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "AssetManager.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Progress(Ptr<AScene> scene)
{
	UINT* matrix = scene->GetCollisionMatrix();

	for (UINT row = 0; row < MAX_LAYER; ++row)
	{
		for (UINT col = row; col < MAX_LAYER; ++col)
		{
			if ((matrix[row] & (1 << col)) == false)
				continue;

			ColisionBtwLayer(scene->GetLayer(row), scene->GetLayer(col));
		}
	}
}

void CollisionManager::ColisionBtwLayer(Layer* left, Layer* right)
{
	const vector<Ptr<GameObject>>& vecLeft = left->GetAllObjects();
	const vector<Ptr<GameObject>>& vecRight = right->GetAllObjects();

	for (size_t i = 0; i < vecLeft.size(); i++)
	{
		if (vecLeft[i]->Collider2D() == nullptr)
			continue;

		for (size_t j = 0; j < vecRight.size(); j++)
		{
			if (vecRight[j]->Collider2D() == nullptr)
				continue;

			COL_ID colid;
			colid.leftID = vecLeft[i]->Collider2D()->GetID();
			colid.rightID = vecRight[j]->Collider2D()->GetID();

			map<ULONGLONG, bool>::iterator iter = m_mapColID.find(colid.ID);
			if (iter == m_mapColID.end())
			{
				m_mapColID.insert({ colid.ID, false });
				iter = m_mapColID.find(colid.ID);
			}

			// 충돌 검사
			if (IsCollision(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
			{
				if (iter->second == true)
				{
					vecLeft[i]->Collider2D()->Overlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->Overlap(vecLeft[i]->Collider2D());
				}
				else if (iter->second == false)
				{
					vecLeft[i]->Collider2D()->BeginOverlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->BeginOverlap(vecLeft[i]->Collider2D());
				}
				iter->second = true;
			}
			else
			{
				if (iter->second == true)
				{
					vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
				}
				iter->second = false;
			}
		}
	}
}

bool CollisionManager::IsCollision(Ptr<CCollider2D> leftColl, Ptr<CCollider2D> rightColl)
{
	Ptr<AMesh> pRecMesh = FIND(AMesh, L"RectMesh");

	const VertexInfo* vertices = pRecMesh->GetVtxSysMem();
	/*const UINT* indices = pRecMesh->GetIdxSysMem();*/

	const Matrix& matWorldLeft = leftColl->GetWorldMat();
	const Matrix& matWorldRight = rightColl->GetWorldMat();
	
	Vec3 dirVecs[4] = {};
	dirVecs[0] = XMVector3TransformCoord(vertices[1].position, matWorldLeft) - XMVector3TransformCoord(vertices[0].position, matWorldLeft);
	dirVecs[1] = XMVector3TransformCoord(vertices[3].position, matWorldLeft) - XMVector3TransformCoord(vertices[0].position, matWorldLeft);
	dirVecs[2] = XMVector3TransformCoord(vertices[1].position, matWorldRight) - XMVector3TransformCoord(vertices[0].position, matWorldRight);
	dirVecs[3] = XMVector3TransformCoord(vertices[3].position, matWorldRight) - XMVector3TransformCoord(vertices[0].position, matWorldRight);
	

	Vec3 center = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matWorldRight) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matWorldLeft);

	for (int i = 0; i < 4; i++)
	{
		Vec3 projAxis = dirVecs[i];
		projAxis.Normalize();

		float sum = 0.f;

		for (int j = 0; j < 4; j++)
		{
			sum += fabs(projAxis.Dot(dirVecs[j]));
		}
		sum /= 2.f;

		float projCenter = fabs(projAxis.Dot(center));

		// 중심 투영보다 각 투영 벡터의 합이 더 크다 -> 겹쳤기 때문에 그렇다.
		if (projCenter > sum)
			return false;
	}

	return true;
}

