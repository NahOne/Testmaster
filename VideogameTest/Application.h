#pragma once
#include "SphericalCameraController.h"

class CDebugRender;
class CContextManager;


class CApplication
{
public:
	CApplication(CDebugRender *_DebugRender, CContextManager *_ContextManager);
	~CApplication();

	void Update(float _ElapsedTime);
	void Render();

private:
	CDebugRender *m_DebugRender;
	CContextManager *m_ContextManager;
	CSphericalCameraController m_Camera;
	float m_WorldRotation;
};

