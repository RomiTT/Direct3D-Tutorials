#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "particleshaderclass.h"
#include "particlesystemclass.h"
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
	if(!m_Direct3D)
	{
		return false;
	}

	// Direct3D ��ü �ʱ�ȭ
	if(!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
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

	// ī�޶� ������ ����
	m_Camera->SetPosition(0.0f, -1.5f, -7.0f);
	
	// ��ƼŬ ���̴� ��ü�� ����ϴ�.
	m_ParticleShader = new ParticleShaderClass;
	if(!m_ParticleShader)
	{
		return false;
	}

	// ��ƼŬ ���̴� ��ü�� �ʱ�ȭ�մϴ�.
	if(!m_ParticleShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// ��ƼŬ �ý��� ��ü�� ����ϴ�.
	m_ParticleSystem = new ParticleSystemClass;
	if(!m_ParticleSystem)
	{
		return false;
	}

	// ��ƼŬ �ý��� ��ü�� �ʱ�ȭ�մϴ�.
	if(!m_ParticleSystem->Initialize(m_Direct3D->GetDevice(), L"../Dx11Demo_39/data/star.dds"))
	{
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// ��ƼŬ �ý��� ��ü�� �����մϴ�.
	if(m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// particle shader ��ü�� �����Ѵ�.
	if(m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	// m_Camera ��ü ��ȯ
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D ��ü ��ȯ
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}


bool GraphicsClass::Frame(float frameTime)
{
	// ��ƼŬ �ý��ۿ� ���� ������ ó���� �����մϴ�.
	m_ParticleSystem->Frame(frameTime, m_Direct3D->GetDeviceContext());

	// �׷��� ������ ó��
	return Render();
}


bool GraphicsClass::Render()
{
	// ���� �׸��� ���� ���۸� ����ϴ�
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// ���� ������ �մϴ�.
	m_Direct3D->EnableAlphaBlending();

	// ��ƼŬ �ý��� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� �׸��⸦ �غ��մϴ�.
	m_ParticleSystem->Render(m_Direct3D->GetDeviceContext());

	// �ؽ�ó ���̴��� ����Ͽ� ���� �������մϴ�.
	if(!m_ParticleShader->Render(m_Direct3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
									  m_ParticleSystem->GetTexture()))
	{
		return false;
	}

	// ���� ������ ���ϴ�.
	m_Direct3D->DisableAlphaBlending();

	// ������ �� ����� ȭ�鿡 ǥ���մϴ�.
	m_Direct3D->EndScene();

	return true;
}