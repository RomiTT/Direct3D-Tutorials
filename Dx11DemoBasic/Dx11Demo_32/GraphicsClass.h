#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class D3DClass;
class CameraClass;
class ModelClass;
class TextureShaderClass;
class RenderTextureClass;
class GlassShaderClass;

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
	bool RenderToTexture(float);
	bool Render(float);

private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	ModelClass* m_Model = nullptr;
	ModelClass* m_WindowModel = nullptr;
	RenderTextureClass* m_RenderTexture = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	GlassShaderClass* m_GlassShader = nullptr;
};