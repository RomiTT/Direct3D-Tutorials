#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "fireshaderclass.h"
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
	// Direct3D ��ü ����
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D ��ü �ʱ�ȭ
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
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

	// �� ��ü ����
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// �� ��ü �ʱ�ȭ
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), "../Dx11Demo_33/data/square.txt", L"../Dx11Demo_33/data/fire01.dds", 
								 L"../Dx11Demo_33/data/noise01.dds", L"../Dx11Demo_33/data/alpha01.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// ���̾� ���̴� ��ü�� �����Ѵ�.
	m_FireShader = new FireShaderClass;
	if(!m_FireShader)
	{
		return false;
	}

	// ȭ�� ���̴� ��ü�� �ʱ�ȭ�մϴ�.
	if(!m_FireShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the fire shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// fire shader ��ü�� �����Ѵ�.
	if (m_FireShader)
	{
		m_FireShader->Shutdown();
		delete m_FireShader;
		m_FireShader = 0;
	}

	// �� ��ü�� �����մϴ�.
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
	// ī�޶� ��ġ�� �����մϴ�.
	m_Camera->SetPosition(0.0f, 0.0f, -3.0f);

	// ����� �������մϴ�.
	return Render();
}


bool GraphicsClass::Render()
{
	static float frameTime = 0.0f;


	// ������ �ð� ī���͸� ������ŵ�ϴ�.
	frameTime += 0.01f;
	if(frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}

	// �� ���� �ٸ� ������ �ؽ�ó�� ���� �� ���� ��ũ�� �ӵ��� �����մϴ�.
	XMFLOAT3 scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);

	// �� ���� ���� �ٸ� ������ ��Ÿ�� �ؽ�ó�� ����� �� ����� �� ���� �������� �����մϴ�.
	XMFLOAT3 scales = XMFLOAT3(1.0f, 2.0f, 3.0f);

	// �� ���� �ٸ� ������ �ؽ�ó�� ���� �� ���� �ٸ� x �� y �ְ� �μ��� �����մϴ�.
	XMFLOAT2 distortion1 = XMFLOAT2(0.1f, 0.2f);
	XMFLOAT2 distortion2 = XMFLOAT2(0.1f, 0.3f);
	XMFLOAT2 distortion3 = XMFLOAT2(0.1f, 0.1f);

	// �ؽ�ó ��ǥ ���ø� ������ �����ϰ� ���̾.
	float distortionScale = 0.8f;
	float distortionBias = 0.5f;

	// ����� ������ ���۸� �����.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�.
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� (ortho) ����� �����ɴϴ�.
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// ȭ�� ������ ���� ���带 �մϴ�.
	m_Direct3D->TurnOnAlphaBlending();

	// ���簢�� ���� ������ �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� �׸��⸦ �غ��մϴ�.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// ȭ�� ���̴��� ����Ͽ� �簢�� ���� �������մϴ�.
	if(!m_FireShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
							 m_Model->GetTexture1(), m_Model->GetTexture2(), m_Model->GetTexture3(), frameTime, scrollSpeeds,
							 scales, distortion1, distortion2, distortion3, distortionScale, distortionBias))
	{
		return false;
	}

	// ���� ������ ���ϴ�.
	m_Direct3D->TurnOffAlphaBlending();

	// ������ �� ����� ȭ�鿡 ǥ���մϴ�.
	m_Direct3D->EndScene();

	return true;
}