#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "rendertextureclass.h"
#include "orthowindowclass.h"
#include "horizontalblurshaderclass.h"
#include "verticalblurshaderclass.h"
#include "glowmapshaderclass.h"
#include "glowshaderclass.h"
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Direct3D 개체를 만듭니다.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	bool result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 카메라의 초기 위치를 초기화합니다.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	
	// 텍스처 쉐이더 객체를 생성한다.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// 텍스처 쉐이더 객체를 초기화한다.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
	
	// 비트 맵 객체를 만듭니다.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
	{
		return false;
	}

	// 비트 맵 객체를 초기화합니다.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Dx11Demo_46/data/test.dds", L"../Dx11Demo_46/data/glowmap.dds", 256, 32);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	
	// 렌더링 텍스처 객체를 생성한다.
	m_RenderTexture = new RenderTextureClass;
	if(!m_RenderTexture)
	{
		return false;
	}

	// 렌더링 텍스처 객체를 초기화한다.
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}

	// 다운 샘플 렌더링을 텍스처 오브젝트에 생성한다.
	m_DownSampleTexure = new RenderTextureClass;
	if(!m_DownSampleTexure)
	{
		return false;
	}

	// 다운 샘플 렌더를 텍스처 오브젝트로 초기화한다.
	result = m_DownSampleTexure->Initialize(m_D3D->GetDevice(), (screenWidth / 2), (screenHeight / 2), SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the down sample render to texture object.", L"Error", MB_OK);
		return false;
	}
	
	// 작은 ortho 윈도우 객체를 만듭니다.
	m_SmallWindow = new OrthoWindowClass;
	if(!m_SmallWindow)
	{
		return false;
	}

	// 작은 ortho 윈도우 객체를 초기화합니다.
	result = m_SmallWindow->Initialize(m_D3D->GetDevice(), (screenWidth / 2), (screenHeight / 2));
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the small ortho window object.", L"Error", MB_OK);
		return false;
	}
	
	// 텍스처 객체에 수평 블러 렌더링을 만듭니다.
	m_HorizontalBlurTexture = new RenderTextureClass;
	if(!m_HorizontalBlurTexture)
	{
		return false;
	}

	// 텍스처 객체에 수평 블러 렌더링을 초기화합니다.
	result = m_HorizontalBlurTexture->Initialize(m_D3D->GetDevice(), (screenWidth / 2), (screenHeight / 2), SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// 수평 블러 쉐이더 객체를 만듭니다.
	m_HorizontalBlurShader = new HorizontalBlurShaderClass;
	if(!m_HorizontalBlurShader)
	{
		return false;
	}

	// 수평 블러 쉐이더 객체를 초기화합니다.
	result = m_HorizontalBlurShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur shader object.", L"Error", MB_OK);
		return false;
	}
	
	// 텍스처 오브젝트에 수직 블러 렌더를 만듭니다.
	m_VerticalBlurTexture = new RenderTextureClass;
	if(!m_VerticalBlurTexture)
	{
		return false;
	}

	// 텍스처 오브젝트에 수직 블러 렌더를 초기화합니다.
	result = m_VerticalBlurTexture->Initialize(m_D3D->GetDevice(), (screenWidth / 2), (screenHeight / 2), SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// 수직 블러 셰이더 오브젝트를 생성한다.
	m_VerticalBlurShader = new VerticalBlurShaderClass;
	if(!m_VerticalBlurShader)
	{
		return false;
	}

	// 수직 블러 쉐이더 객체를 초기화합니다.
	result = m_VerticalBlurShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur shader object.", L"Error", MB_OK);
		return false;
	}

	// 텍스쳐 객체에 업 샘플 렌더를 생성한다.
	m_UpSampleTexure = new RenderTextureClass;
	if(!m_UpSampleTexure)
	{
		return false;
	}

	// up 샘플 렌더를 텍스처 오브젝트로 초기화한다.
	result = m_UpSampleTexure->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the up sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	// 전체 화면 ortho window 객체를 만듭니다.
	m_FullScreenWindow = new OrthoWindowClass;
	if(!m_FullScreenWindow)
	{
		return false;
	}

	// 전체 화면 ortho window 객체를 초기화합니다.
	result = m_FullScreenWindow->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
		return false;
	}

	// 글로우 맵 셰이더 개체를 만듭니다.
	m_GlowMapShader = new GlowMapShaderClass;
	if(!m_GlowMapShader)
	{
		return false;
	}

	// 글로우 맵 쉐이더 객체를 초기화합니다.
	result = m_GlowMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the glow map shader object.", L"Error", MB_OK);
		return false;
	}

	// 글로우 쉐이더 객체를 만듭니다.
	m_GlowShader = new GlowShaderClass;
	if(!m_GlowShader)
	{
		return false;
	}

	// 글로우 쉐이더 객체를 초기화합니다.
	result = m_GlowShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the glow shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// 글로우 쉐이더 객체를 해제합니다.
	if(m_GlowShader)
	{
		m_GlowShader->Shutdown();
		delete m_GlowShader;
		m_GlowShader = 0;
	}

	// 글로우 맵 셰이더 객체를 해제합니다.
	if(m_GlowMapShader)
	{
		m_GlowMapShader->Shutdown();
		delete m_GlowMapShader;
		m_GlowMapShader = 0;
	}

	// 전체 화면 ortho window 객체를 해제합니다.
	if(m_FullScreenWindow)
	{
		m_FullScreenWindow->Shutdown();
		delete m_FullScreenWindow;
		m_FullScreenWindow = 0;
	}

	// up 샘플 렌더를 텍스쳐 객체로 릴리즈한다.
	if(m_UpSampleTexure)
	{
		m_UpSampleTexure->Shutdown();
		delete m_UpSampleTexure;
		m_UpSampleTexure = 0;
	}

	// 수직 블러 쉐이더 객체를 해제한다.
	if(m_VerticalBlurShader)
	{
		m_VerticalBlurShader->Shutdown();
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}

	// 수직 블러 렌더를 텍스처 객체에 놓습니다.
	if(m_VerticalBlurTexture)
	{
		m_VerticalBlurTexture->Shutdown();
		delete m_VerticalBlurTexture;
		m_VerticalBlurTexture = 0;
	}

	// 수평 블러 쉐이더 객체를 해제합니다.
	if(m_HorizontalBlurShader)
	{
		m_HorizontalBlurShader->Shutdown();
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	// 수평 블러 렌더를 텍스처 객체에 놓습니다.
	if(m_HorizontalBlurTexture)
	{
		m_HorizontalBlurTexture->Shutdown();
		delete m_HorizontalBlurTexture;
		m_HorizontalBlurTexture = 0;
	}

	// 작은 ortho 윈도우 객체를 놓습니다.
	if(m_SmallWindow)
	{
		m_SmallWindow->Shutdown();
		delete m_SmallWindow;
		m_SmallWindow = 0;
	}

	// 다운 샘플 렌더를 텍스쳐 객체로 릴리즈한다.
	if(m_DownSampleTexure)
	{
		m_DownSampleTexure->Shutdown();
		delete m_DownSampleTexure;
		m_DownSampleTexure = 0;
	}

	// 렌더를 텍스쳐 객체로 릴리즈한다.
	if(m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// 비트 맵 객체를 해제합니다.
	if(m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// 텍스처 쉐이더 객체를 해제한다.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// 카메라 객체를 해제합니다.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D 객체 반환
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
}


bool GraphicsClass::Frame()
{
	// 그래픽 장면을 렌더링합니다.
	return Render();
}


bool GraphicsClass::Render()
{
	// 글로우 맵을 렌더링 텍스처에 먼저 렌더링합니다.
	bool result = RenderGlowMapToTexture();
	if(!result)
	{
		return false;
	}

	// 다음으로 렌더 텍스처를 작은 크기의 텍스처로 샘플링합니다.
	result = DownSampleTexture();
	if (!result)
	{
		return false;
	}

	// 다운 샘플링 된 렌더링 텍스처에 수평 블러를 수행합니다.
	result = RenderHorizontalBlurToTexture();
	if (!result)
	{
		return false;
	}

	// 이제 수평 흐림 렌더 텍스처에서 수직 흐림 효과를 수행합니다.
	result = RenderVerticalBlurToTexture();
	if (!result)
	{
		return false;
	}

	// 최종 흐린 렌더링 텍스처를 화면 크기로 다시 샘플링합니다.
	result = UpSampleTexture();
	if (!result)
	{
		return false;
	}

	// 일반 UI 요소를 전체 화면 텍스처로 렌더링합니다.
	result = RenderUIElementsToTexture();
	if(!result)
	{
		return false;
	}
	
	// UI 요소와 빛나는 UI 요소를 결합한 최종 장면을 렌더링합니다.
	RenderGlowScene();

	return true;
}


bool GraphicsClass::RenderGlowMapToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	
	// 렌더링 대상을 렌더링에 맞게 설정합니다.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// 렌더링을 텍스처에 지웁니다.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_D3D->TurnZBufferOff();

	// 비트 맵 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	if(!m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100))
	{
		return false;
	}

	// 글로우 맵 셰이더를 사용하여 비트 맵을 렌더링합니다.
	m_GlowMapShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, 
							m_Bitmap->GetTexture(), m_Bitmap->GetGlowMap());

	//모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_D3D->TurnZBufferOn();

	// 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
	m_D3D->SetBackBufferRenderTarget();

	// 뷰포트를 원본으로 다시 설정합니다.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::DownSampleTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// 렌더링 대상을 렌더링에 맞게 설정합니다.
	m_DownSampleTexure->SetRenderTarget(m_D3D->GetDeviceContext());

	// 렌더링을 텍스처에 지웁니다.
	m_DownSampleTexure->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	// 카메라와 d3d 객체로부터 월드와 뷰 매트릭스를 얻는다.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	
	// 질감이 크기가 작기 때문에 렌더링에서 텍스처로 ortho 행렬을 가져옵니다.
	m_DownSampleTexure->GetOrthoMatrix(orthoMatrix);

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_D3D->TurnZBufferOff();

	// 드로잉을 준비하기 위해 그래픽 파이프 라인에 작은 ortho window 버텍스와 인덱스 버퍼를 놓습니다.
	m_SmallWindow->Render(m_D3D->GetDeviceContext());

	// 텍스처 쉐이더를 사용하여 작은 ortho 창을 렌더링하고 씬의 텍스처를 텍스처 리소스로 렌더링합니다.
	if(!m_TextureShader->Render(m_D3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_RenderTexture->GetShaderResourceView()))
	{
		return false;
	}

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_D3D->TurnZBufferOn();
	
	// 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
	m_D3D->SetBackBufferRenderTarget();

	// 뷰포트를 원본으로 다시 설정합니다.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::RenderHorizontalBlurToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	
	// 렌더링 대상을 렌더링에 맞게 설정합니다.
	m_HorizontalBlurTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// 렌더링을 텍스처에 지웁니다.
	m_HorizontalBlurTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	// 카메라와 d3d 객체로부터 월드와 뷰 매트릭스를 얻는다.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	// 텍스쳐가 다른 차원을 가지므로 렌더링에서 오쏘 (ortho) 행렬을 텍스처로 가져옵니다.
	m_HorizontalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_D3D->TurnZBufferOff();

	// 수평 블러 쉐이더에서 사용될 float에 화면 폭을 저장합니다.
	float screenSizeX = (float)m_HorizontalBlurTexture->GetTextureWidth();
	
	// 드로잉을 준비하기 위해 그래픽 파이프 라인에 작은 ortho window 버텍스와 인덱스 버퍼를 놓습니다.
	m_SmallWindow->Render(m_D3D->GetDeviceContext());
	
	// horizontal blur shader와 down sampled render를 사용하여 작은 ortho 윈도우를 텍스처 리소스로 렌더링합니다.	
	if(!m_HorizontalBlurShader->Render(m_D3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
										m_DownSampleTexure->GetShaderResourceView(), screenSizeX))
	{
		return false;
	}

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_D3D->TurnZBufferOn();

	// 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
	m_D3D->SetBackBufferRenderTarget();

	// 뷰포트를 원본으로 다시 설정합니다.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::RenderVerticalBlurToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	
	// 렌더링 대상을 렌더링에 맞게 설정합니다.
	m_VerticalBlurTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// 렌더링을 텍스처에 지웁니다.
	m_VerticalBlurTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	// 카메라와 d3d 객체로부터 월드와 뷰 매트릭스를 얻는다.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	// 텍스쳐가 다른 차원을 가지므로 렌더링에서 오쏘 (ortho) 행렬을 텍스처로 가져옵니다.
	m_VerticalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_D3D->TurnZBufferOff();

	// 수직 블러 셰이더에서 사용되는 부동 소수점에 화면 높이를 저장합니다.
	float screenSizeY = (float)m_VerticalBlurTexture->GetTextureHeight();
	
	// 드로잉을 준비하기 위해 그래픽 파이프 라인에 작은 ortho window 버텍스와 인덱스 버퍼를 놓습니다.
	m_SmallWindow->Render(m_D3D->GetDeviceContext());
	
	// 수직 블러 쉐이더와 수평 블러 링을 사용하여 작은 ortho 윈도우를 텍스처 리소스로 렌더합니다.
	if(!m_VerticalBlurShader->Render(m_D3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
									m_HorizontalBlurTexture->GetShaderResourceView(), screenSizeY))
	{
		return false;
	}

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_D3D->TurnZBufferOn();

	// 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
	m_D3D->SetBackBufferRenderTarget();

	// 뷰포트를 원본으로 다시 설정합니다.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::UpSampleTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// 렌더링 대상을 렌더링에 맞게 설정합니다.
	m_UpSampleTexure->SetRenderTarget(m_D3D->GetDeviceContext());

	// 렌더링을 텍스처에 지웁니다.
	m_UpSampleTexure->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	// 카메라와 d3d 객체로부터 월드와 뷰 매트릭스를 얻는다.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);

	// 텍스쳐가 다른 차원을 가지므로 렌더링에서 오쏘 (ortho) 행렬을 텍스처로 가져옵니다.
	m_UpSampleTexure->GetOrthoMatrix(orthoMatrix);

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_D3D->TurnZBufferOff();

	// 그래픽 파이프 라인에 전체 화면 직교 윈도우 버텍스와 인덱스 버퍼를 배치하여 그리기를 준비합니다.
	m_FullScreenWindow->Render(m_D3D->GetDeviceContext());

	// 텍스처 쉐이더와 텍스처 리소스에 대한 작은 크기의 최종 흐리게 렌더링을 사용하여 전체 화면 ortho 창을 렌더링합니다.
	if(!m_TextureShader->Render(m_D3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_VerticalBlurTexture->GetShaderResourceView()))
	{
		return false;
	}

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_D3D->TurnZBufferOn();
	
	// 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
	m_D3D->SetBackBufferRenderTarget();

	// 뷰포트를 원본으로 다시 설정합니다.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::RenderUIElementsToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;


	// 렌더링 대상을 렌더링에 맞게 설정합니다.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// 렌더링을 텍스처에 지웁니다.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 오쏘 (ortho) 행렬을 가져옵니다.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_D3D->TurnZBufferOff();

	// 비트 맵 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100);
	if(!result)
	{
		return false;
	}

	// 텍스처 쉐이더를 사용하여 비트 맵을 렌더링합니다.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if(!result)
	{
		return false;
	}

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_D3D->TurnZBufferOn();

	// 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
	m_D3D->SetBackBufferRenderTarget();

	// 뷰포트를 원본으로 다시 설정합니다.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::RenderGlowScene()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// 장면을 시작할 버퍼를 지운다.
	m_D3D->BeginScene(1.0f, 0.0f, 0.0f, 0.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 오쏘 (ortho) 행렬을 가져옵니다.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_D3D->TurnZBufferOff();

	// 그래픽 파이프 라인에 전체 화면 직교 윈도우 버텍스와 인덱스 버퍼를 배치하여 그리기를 준비합니다.
	m_FullScreenWindow->Render(m_D3D->GetDeviceContext());

	// 텍스처 쉐이더를 사용하여 전체 화면 ortho 창을 렌더링하고 텍스처 리소스에 전체 화면 크기로 흐리게 렌더링합니다.
	m_GlowShader->Render(m_D3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, 
						 m_RenderTexture->GetShaderResourceView(), m_UpSampleTexure->GetShaderResourceView(), 3.0f);

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_D3D->TurnZBufferOn();
	
	// 렌더링 된 장면을 화면에 표시합니다.
	m_D3D->EndScene();

	return true;
}