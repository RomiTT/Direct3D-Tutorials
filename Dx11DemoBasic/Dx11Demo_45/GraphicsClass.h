#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class D3DClass;
class ShaderManagerClass;
class CameraClass;
class ModelClass;
class LightClass;
class BumpModelClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float);

private:
	D3DClass* m_D3D = nullptr;
	ShaderManagerClass* m_ShaderManager = nullptr;
	CameraClass* m_Camera = nullptr;
	LightClass* m_Light = nullptr;
	ModelClass *m_Model1 = nullptr;
	ModelClass *m_Model2 = nullptr;
	BumpModelClass* m_Model3 = nullptr;
};