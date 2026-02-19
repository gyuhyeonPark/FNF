#pragma once
#include "SingletonClass.h"
#include "GameObject.h"
#include "StructuredBuffer.h"

class RenderManager : public SingletonClass<RenderManager>
{
	SINGLE(RenderManager);
public:
	virtual ~RenderManager();

public:
	void Init();
	void Progress();

public:
	void RegisterCamera(Ptr<CCamera> cam) { m_mainCamera = cam; }
	Ptr<CCamera> GetPOVCamera() { return m_mainCamera; }
	void AddDebugInfo(const DbgInfo& info) 
	{
		if (m_debugOption)
			m_dbgInfoList.push_back(info);
	}
	void RegisterLight2D(Ptr<CLight2D> _Light2D) { m_vecLight2D.push_back(_Light2D); }

public:
	void Render_Debug();
	void Render_Start();
	void Render_End();

private:
	Ptr<CCamera> m_mainCamera;
	Ptr<GameObject> m_dbgObj;
	list<DbgInfo> m_dbgInfoList;

	vector<Ptr<CLight2D>> m_vecLight2D;
	Ptr<StructuredBuffer> m_light2DBuffer;

	bool m_debugOption;
};

