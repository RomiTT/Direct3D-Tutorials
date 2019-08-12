#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"
#include "horizontalblurshaderclass.h"
#include "verticalblurshaderclass.h"
#include "rendertextureclass.h"
#include "orthowindowclass.h"
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
	bool result = false;

	// ũ�⸦ ���÷� �����մϴ�.
	int downSampleWidth = screenWidth / 2;
	int downSampleHeight = screenHeight / 2;

	// Direct3D ��ü�� ����ϴ�.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Direct3D ��ü �ʱ�ȭ
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera ��ü ����
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// ī�޶��� �ʱ� ��ġ�� �ʱ�ȭ�մϴ�.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	
	// �� ��ü ����
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// �� ��ü �ʱ�ȭ
	result = m_Model->Initialize(m_Direct3D->GetDevice(), "../Dx11Demo_36/data/cube.txt", L"../Dx11Demo_36/data/seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	
	// �ؽ�ó ���̴� ��ü�� �����Ѵ�.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// �ؽ�ó ���̴� ��ü�� �ʱ�ȭ�Ѵ�.
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// ���� �� ���̴� ��ü�� ����ϴ�.
	m_HorizontalBlurShader = new HorizontalBlurShaderClass;
	if(!m_HorizontalBlurShader)
	{
		return false;
	}

	// ���� �� ���̴� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_HorizontalBlurShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur shader object.", L"Error", MB_OK);
		return false;
	}

	// ���� �� ���̴� ������Ʈ�� �����Ѵ�.
	m_VerticalBlurShader = new VerticalBlurShaderClass;
	if(!m_VerticalBlurShader)
	{
		return false;
	}

	// ���� �� ���̴� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_VerticalBlurShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur shader object.", L"Error", MB_OK);
		return false;
	}

	// ������ �ؽ�ó ��ü�� �����Ѵ�.
	m_RenderTexture = new RenderTextureClass;
	if(!m_RenderTexture)
	{
		return false;
	}

	// ������ �ؽ�ó ��ü�� �ʱ�ȭ�Ѵ�.
	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}

	// �ٿ� ���� �������� �ؽ�ó ������Ʈ�� �����Ѵ�.
	m_DownSampleTexure = new RenderTextureClass;
	if(!m_DownSampleTexure)
	{
		return false;
	}

	// �ٿ� ���� ������ �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�.
	result = m_DownSampleTexure->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the down sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	// �ؽ�ó ��ü�� ���� �� �������� ����ϴ�.
	m_HorizontalBlurTexture = new RenderTextureClass;
	if(!m_HorizontalBlurTexture)
	{
		return false;
	}

	// �ؽ�ó ��ü�� ���� �� �������� �ʱ�ȭ�մϴ�.
	result = m_HorizontalBlurTexture->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// �ؽ�ó ������Ʈ�� ���� �� ������ ����ϴ�.
	m_VerticalBlurTexture = new RenderTextureClass;
	if(!m_VerticalBlurTexture)
	{
		return false;
	}

	// �ؽ�ó ������Ʈ�� ���� �� ������ �ʱ�ȭ�մϴ�.
	result = m_VerticalBlurTexture->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// �ؽ��� ��ü�� �� ���� ������ �����Ѵ�.
	m_UpSampleTexure = new RenderTextureClass;
	if(!m_UpSampleTexure)
	{
		return false;
	}

	// up ���� ������ �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�.
	result = m_UpSampleTexure->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the up sample render to texture object.", L"Error", MB_OK);
		return false;
	}
	
	// ���� ortho ������ ��ü�� ����ϴ�.
	m_SmallWindow = new OrthoWindowClass;
	if(!m_SmallWindow)
	{
		return false;
	}

	// ���� ortho ������ ��ü�� �ʱ�ȭ�մϴ�.
	result = m_SmallWindow->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the small ortho window object.", L"Error", MB_OK);
		return false;
	}

	// ��ü ȭ�� ortho window ��ü�� ����ϴ�.
	m_FullScreenWindow = new OrthoWindowClass;
	if(!m_FullScreenWindow)
	{
		return false;
	}

	// ��ü ȭ�� ortho window ��ü�� �ʱ�ȭ�մϴ�.
	result = m_FullScreenWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// ��ü ȭ�� ortho window ��ü�� �����մϴ�.
	if (m_FullScreenWindow)
	{
		m_FullScreenWindow->Shutdown();
		delete m_FullScreenWindow;
		m_FullScreenWindow = 0;
	}

	// ���� ortho ������ ��ü�� �����ϴ�.
	if (m_SmallWindow)
	{
		m_SmallWindow->Shutdown();
		delete m_SmallWindow;
		m_SmallWindow = 0;
	}

	// up ���� ������ �ؽ��� ��ü�� �������Ѵ�.
	if (m_UpSampleTexure)
	{
		m_UpSampleTexure->Shutdown();
		delete m_UpSampleTexure;
		m_UpSampleTexure = 0;
	}

	// ���� �� ������ �ؽ�ó ��ü�� �����ϴ�.
	if (m_VerticalBlurTexture)
	{
		m_VerticalBlurTexture->Shutdown();
		delete m_VerticalBlurTexture;
		m_VerticalBlurTexture = 0;
	}

	// ���� �� ������ �ؽ�ó ��ü�� �����ϴ�.
	if (m_HorizontalBlurTexture)
	{
		m_HorizontalBlurTexture->Shutdown();
		delete m_HorizontalBlurTexture;
		m_HorizontalBlurTexture = 0;
	}

	// �ٿ� ���� ������ �ؽ��� ��ü�� �������Ѵ�.
	if (m_DownSampleTexure)
	{
		m_DownSampleTexure->Shutdown();
		delete m_DownSampleTexure;
		m_DownSampleTexure = 0;
	}

	// ������ �ؽ��� ��ü�� �������Ѵ�.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// ���� �� ���̴� ��ü�� �����Ѵ�.
	if (m_VerticalBlurShader)
	{
		m_VerticalBlurShader->Shutdown();
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}

	// ���� �� ���̴� ��ü�� �����մϴ�.
	if (m_HorizontalBlurShader)
	{
		m_HorizontalBlurShader->Shutdown();
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	// ������ �ؽ��� ��ü�� �������Ѵ�.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}
	
	// Release the vertical blur shader object.
	if(m_VerticalBlurShader)
	{
		m_VerticalBlurShader->Shutdown();
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}

	// Release the horizontal blur shader object.
	if(m_HorizontalBlurShader)
	{
		m_HorizontalBlurShader->Shutdown();
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// �� ��ü ��ȯ
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// m_Camera ��ü ��ȯ
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D ��ü ��ȯ
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}


bool GraphicsClass::Frame()
{
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	return Render(rotation);
}


bool GraphicsClass::Render(float rotation)
{
	bool result = false;

	// ��ü ����� ���� �ؽ�ó�� �������մϴ�.
	result = RenderSceneToTexture(rotation);
	if(!result)
	{
		return false;
	}

	// �������� ���� �ؽ�ó�� ���� ũ���� �ؽ�ó�� ���ø��մϴ�.
	result = DownSampleTexture();
	if (!result)
	{
		return false;
	}

	// �ٿ� ���ø� �� ������ �ؽ�ó�� ���� ���� �����մϴ�.
	result = RenderHorizontalBlurToTexture();
	if (!result)
	{
		return false;
	}

	// ���� ���� �帲 ���� �ؽ�ó���� ���� �帲 ȿ���� �����մϴ�.
	result = RenderVerticalBlurToTexture();
	if (!result)
	{
		return false;
	}

	// ���� �帰 ������ �ؽ�ó�� ȭ�� ũ��� �ٽ� ���ø��մϴ�.
	result = UpSampleTexture();
	if (!result)
	{
		return false;
	}

	// �帮�� ���ø� �� ������ �ؽ�ó�� ȭ�鿡 �������մϴ�.
	result = Render2DTextureScene();
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::RenderSceneToTexture(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	
	// ������ ����� �������� �°� �����մϴ�.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// �������� �ؽ�ó�� ����ϴ�.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�.
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// ȸ�� ������ ���� ����� ȸ���Ͽ� ť�갡 ȸ���ϵ����մϴ�.
	worldMatrix = XMMatrixRotationY(rotation);

	// �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������� �غ��մϴ�.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// �ؽ�ó ���̴��� ����Ͽ� ���� �������մϴ�.
	if(!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture()))
	{
		return false;
	}

	// ������ ����� ������ �� ���۷� �ٽ� �����ϰ� �������� ���� �������� �� �̻� �ٽ� �������� �ʽ��ϴ�.
	m_Direct3D->SetBackBufferRenderTarget();

	// ����Ʈ�� �������� �ٽ� �����մϴ�.
	m_Direct3D->ResetViewport();

	return true;
}


bool GraphicsClass::DownSampleTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// ������ ����� �������� �°� �����մϴ�.
	m_DownSampleTexure->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// �������� �ؽ�ó�� ����ϴ�.
	m_DownSampleTexure->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�.
	m_Camera->Render();

	// ī�޶�� d3d ��ü�κ��� ����� �� ��Ʈ������ ��´�.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	
	// ������ ũ�Ⱑ �۱� ������ ���������� �ؽ�ó�� ortho ����� �����ɴϴ�.
	m_DownSampleTexure->GetOrthoMatrix(orthoMatrix);

	// ��� 2D �������� �����Ϸ��� Z ���۸� ���ϴ�.
	m_Direct3D->TurnZBufferOff();

	// ������� �غ��ϱ� ���� �׷��� ������ ���ο� ���� ortho window ���ؽ��� �ε��� ���۸� �����ϴ�.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// �ؽ�ó ���̴��� ����Ͽ� ���� ortho â�� �������ϰ� ���� �ؽ�ó�� �ؽ�ó ���ҽ��� �������մϴ�.
	if(!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_RenderTexture->GetShaderResourceView()))
	{
		return false;
	}

	// ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
	m_Direct3D->TurnZBufferOn();
	
	// ������ ����� ������ �� ���۷� �ٽ� �����ϰ� �������� ���� �������� �� �̻� �ٽ� �������� �ʽ��ϴ�.
	m_Direct3D->SetBackBufferRenderTarget();

	// ����Ʈ�� �������� �ٽ� �����մϴ�.
	m_Direct3D->ResetViewport();

	return true;
}


bool GraphicsClass::RenderHorizontalBlurToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// ���� �� ���̴����� ���� float�� ȭ�� ���� �����մϴ�.
	float screenSizeX = (float)m_HorizontalBlurTexture->GetTextureWidth();
	
	// ������ ����� �������� �°� �����մϴ�.
	m_HorizontalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// �������� �ؽ�ó�� ����ϴ�.
	m_HorizontalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�.
	m_Camera->Render();

	// ī�޶�� d3d ��ü�κ��� ����� �� ��Ʈ������ ��´�.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// �ؽ��İ� �ٸ� ������ �����Ƿ� ���������� ���� (ortho) ����� �ؽ�ó�� �����ɴϴ�.
	m_HorizontalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// ��� 2D �������� �����Ϸ��� Z ���۸� ���ϴ�.
	m_Direct3D->TurnZBufferOff();

	// ������� �غ��ϱ� ���� �׷��� ������ ���ο� ���� ortho window ���ؽ��� �ε��� ���۸� �����ϴ�.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());
	
	// horizontal blur shader�� down sampled render�� ����Ͽ� ���� ortho �����츦 �ؽ�ó ���ҽ��� �������մϴ�.	
	if(!m_HorizontalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
										m_DownSampleTexure->GetShaderResourceView(), screenSizeX))
	{
		return false;
	}

	// ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
	m_Direct3D->TurnZBufferOn();

	// ������ ����� ������ �� ���۷� �ٽ� �����ϰ� �������� ���� �������� �� �̻� �ٽ� �������� �ʽ��ϴ�.
	m_Direct3D->SetBackBufferRenderTarget();

	// ����Ʈ�� �������� �ٽ� �����մϴ�.
	m_Direct3D->ResetViewport();

	return true;
}


bool GraphicsClass::RenderVerticalBlurToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// ���� �� ���̴����� ���Ǵ� �ε� �Ҽ����� ȭ�� ���̸� �����մϴ�.
	float screenSizeY = (float)m_VerticalBlurTexture->GetTextureHeight();
	
	// ������ ����� �������� �°� �����մϴ�.
	m_VerticalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// �������� �ؽ�ó�� ����ϴ�.
	m_VerticalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�.
	m_Camera->Render();

	// ī�޶�� d3d ��ü�κ��� ����� �� ��Ʈ������ ��´�.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// �ؽ��İ� �ٸ� ������ �����Ƿ� ���������� ���� (ortho) ����� �ؽ�ó�� �����ɴϴ�.
	m_VerticalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// ��� 2D �������� �����Ϸ��� Z ���۸� ���ϴ�.
	m_Direct3D->TurnZBufferOff();

	// ������� �غ��ϱ� ���� �׷��� ������ ���ο� ���� ortho window ���ؽ��� �ε��� ���۸� �����ϴ�.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());
	
	// ���� �� ���̴��� ���� �� ���� ����Ͽ� ���� ortho �����츦 �ؽ�ó ���ҽ��� �����մϴ�.
	if(!m_VerticalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
									m_HorizontalBlurTexture->GetShaderResourceView(), screenSizeY))
	{
		return false;
	}

	// ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
	m_Direct3D->TurnZBufferOn();

	// ������ ����� ������ �� ���۷� �ٽ� �����ϰ� �������� ���� �������� �� �̻� �ٽ� �������� �ʽ��ϴ�.
	m_Direct3D->SetBackBufferRenderTarget();

	// ����Ʈ�� �������� �ٽ� �����մϴ�.
	m_Direct3D->ResetViewport();

	return true;
}


bool GraphicsClass::UpSampleTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// ������ ����� �������� �°� �����մϴ�.
	m_UpSampleTexure->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// �������� �ؽ�ó�� ����ϴ�.
	m_UpSampleTexure->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�.
	m_Camera->Render();

	// ī�޶�� d3d ��ü�κ��� ����� �� ��Ʈ������ ��´�.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// �ؽ��İ� �ٸ� ������ �����Ƿ� ���������� ���� (ortho) ����� �ؽ�ó�� �����ɴϴ�.
	m_UpSampleTexure->GetOrthoMatrix(orthoMatrix);

	// ��� 2D �������� �����Ϸ��� Z ���۸� ���ϴ�.
	m_Direct3D->TurnZBufferOff();

	// �׷��� ������ ���ο� ��ü ȭ�� ���� ������ ���ؽ��� �ε��� ���۸� ��ġ�Ͽ� �׸��⸦ �غ��մϴ�.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// �ؽ�ó ���̴��� �ؽ�ó ���ҽ��� ���� ���� ũ���� ���� �帮�� �������� ����Ͽ� ��ü ȭ�� ortho â�� �������մϴ�.
	if(!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_VerticalBlurTexture->GetShaderResourceView()))
	{
		return false;
	}

	// ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
	m_Direct3D->TurnZBufferOn();
	
	// ������ ����� ������ �� ���۷� �ٽ� �����ϰ� �������� ���� �������� �� �̻� �ٽ� �������� �ʽ��ϴ�.
	m_Direct3D->SetBackBufferRenderTarget();

	// ����Ʈ�� �������� �ٽ� �����մϴ�.
	m_Direct3D->ResetViewport();

	return true;
}


bool GraphicsClass::Render2DTextureScene()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// ����� ������ ���۸� �����.
	m_Direct3D->BeginScene(1.0f, 0.0f, 0.0f, 0.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�.
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� (ortho) ����� �����ɴϴ�.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// ��� 2D �������� �����Ϸ��� Z ���۸� ���ϴ�.
	m_Direct3D->TurnZBufferOff();

	// �׷��� ������ ���ο� ��ü ȭ�� ���� ������ ���ؽ��� �ε��� ���۸� ��ġ�Ͽ� �׸��⸦ �غ��մϴ�.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// �ؽ�ó ���̴��� ����Ͽ� ��ü ȭ�� ortho â�� �������ϰ� �ؽ�ó ���ҽ��� ��ü ȭ�� ũ��� �帮�� �������մϴ�.
	if(!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_UpSampleTexure->GetShaderResourceView()))
	{
		return false;
	}

	// ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
	m_Direct3D->TurnZBufferOn();
	
	// ������ �� ����� ȭ�鿡 ǥ���մϴ�.
	m_Direct3D->EndScene();

	return true;
}