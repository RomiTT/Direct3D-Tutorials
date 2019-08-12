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
class TextureShaderClass;
class BitmapClass;
class RenderTextureClass;
class OrthoWindowClass;
class HorizontalBlurShaderClass;
class VerticalBlurShaderClass;
class GlowMapShaderClass;
class GlowShaderClass;


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
	bool Render();
	bool RenderGlowMapToTexture();
	bool DownSampleTexture();
	bool RenderHorizontalBlurToTexture();
	bool RenderVerticalBlurToTexture();
	bool UpSampleTexture();
	bool RenderUIElementsToTexture();
	bool RenderGlowScene();

private:
	D3DClass* m_D3D = nullptr;
	CameraClass* m_Camera = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	BitmapClass* m_Bitmap = nullptr;
	RenderTextureClass *m_RenderTexture = nullptr;
	RenderTextureClass *m_DownSampleTexure = nullptr;
	RenderTextureClass *m_HorizontalBlurTexture = nullptr;
	RenderTextureClass *m_VerticalBlurTexture = nullptr;
	RenderTextureClass *m_UpSampleTexure = nullptr;
	OrthoWindowClass *m_SmallWindow = nullptr;
	OrthoWindowClass *m_FullScreenWindow = nullptr;
	HorizontalBlurShaderClass* m_HorizontalBlurShader = nullptr;
	VerticalBlurShaderClass* m_VerticalBlurShader = nullptr;
	GlowMapShaderClass* m_GlowMapShader = nullptr;
	GlowShaderClass* m_GlowShader = nullptr;
};