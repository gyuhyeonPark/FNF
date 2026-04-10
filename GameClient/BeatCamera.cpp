#include "pch.h"
#include "BeatCamera.h"
#include "SongManager.h"
#include "source\\Scripts\\CDotween.h"

BeatCamera::BeatCamera()
	: CScript(-1), m_elapsed(0.f)
{
}

BeatCamera::BeatCamera(const BeatCamera& _origin)
	: CScript(_origin), m_elapsed(0.f)
{
}

BeatCamera::~BeatCamera()
{
}

void BeatCamera::Tick()
{
	m_elapsed += DT;

	float oneBak = 60.f / BPM;

	if (m_elapsed >= oneBak * 2)
	{
		Vec3 currentPos = Transform()->GetRelativePosition();
		Vec3 zoomPos = Vec3(currentPos.x, currentPos.y, currentPos.z * 1.05f);
		GetOwner()->GetScript<CDotween>()->DOMove(zoomPos, oneBak / 2)
			->OnComplete([=]()
				{
					GetOwner()->GetScript<CDotween>()->DOMove(currentPos, oneBak / 2);
				});
		m_elapsed -= oneBak * 2;
	}

}
