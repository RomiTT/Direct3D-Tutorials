#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightclass.h"
#include "projectionshaderclass.h"
#include "textureclass.h"
#include "viewpointclass.h"
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
	bool result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
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

	// ī�޶��� �ʱ� ��ġ�� ȸ���� �����մϴ�.
	m_Camera->SetPosition(XMFLOAT3(0.0f, 7.0f, -10.0f));
	m_Camera->SetRotation(XMFLOAT3(35.0f, 0.0f, 0.0f));
	
	// �׶��� �� ��ü�� ����ϴ�.
	m_GroundModel = new ModelClass;
	if(!m_GroundModel)
	{
		return false;
	}

	// ���� �� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_GroundModel->Initialize(m_Direct3D->GetDevice(), "../Dx11Demo_44/data/floor.txt", L"../Dx11Demo_44/data/stone.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}


	// ť�� �� ��ü�� �����մϴ�.
	m_CubeModel = new ModelClass;
	if(!m_CubeModel)
	{
		return false;
	}

	// ť�� �� ��ü�� �ʱ�ȭ �մϴ�.
	result = m_CubeModel->Initialize(m_Direct3D->GetDevice(), "../Dx11Demo_44/data/cube.txt", L"../Dx11Demo_44/data/seafloor.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the cube model object.", L"Error", MB_OK);
		return false;
	}

	// ���� ��ü�� �����մϴ�.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// ���� ��ü�� �ʱ�ȭ �մϴ�.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetPosition(2.0f, 5.0f, -2.0f);

	// �������� ���̴� ��ü�� ����ϴ�.
	m_ProjectionShader = new ProjectionShaderClass;
	if(!m_ProjectionShader)
	{
		return false;
	}

	// �������� ���̴� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_ProjectionShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the projection shader object.", L"Error", MB_OK);
		return false;
	}

	// ���� �ؽ�ó ��ü�� ����ϴ�.
	m_ProjectionTexture = new TextureClass;
	if(!m_ProjectionTexture)
	{
		return false;
	}

	// ���� �ؽ�ó ��ü�� �ʱ�ȭ�մϴ�.
	result = m_ProjectionTexture->Initialize(m_Direct3D->GetDevice(), L"../Dx11Demo_44/data/grate.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the projection texture object.", L"Error", MB_OK);
		return false;
	}

	// �� ����Ʈ ��ü�� ����ϴ�.
	m_ViewPoint = new ViewPointClass;
	if(!m_ViewPoint)
	{
		return false;
	}

	// �� ����Ʈ ��ü�� �ʱ�ȭ�մϴ�.
	m_ViewPoint->SetPosition(2.0f, 5.0f, -2.0f);
	m_ViewPoint->SetLookAt(0.0f, 0.0f, 0.0f);
	m_ViewPoint->SetProjectionParameters((float)(XM_PI / 2.0f), 1.0f, 0.1f, 100.0f);
	m_ViewPoint->GenerateViewMatrix();
	m_ViewPoint->GenerateProjectionMatrix();

	return true;
}


void GraphicsClass::Shutdown()
{
	// �� ����Ʈ ��ü�� �����մϴ�.
	if(m_ViewPoint)
	{
		delete m_ViewPoint;
		m_ViewPoint = 0;
	}

	// ���� �ؽ�ó ��ü�� �����մϴ�.
	if(m_ProjectionTexture)
	{
		m_ProjectionTexture->Shutdown();
		delete m_ProjectionTexture;
		m_ProjectionTexture = 0;
	}

	// ���� ���̴� ��ü�� �����մϴ�.
	if(m_ProjectionShader)
	{
		m_ProjectionShader->Shutdown();
		delete m_ProjectionShader;
		m_ProjectionShader = 0;
	}

	// light ������Ʈ�� �����Ѵ�.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// ť�� �� ������Ʈ�� �����Ͻʽÿ�.
	if(m_CubeModel)
	{
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = 0;
	}

	// �׶��� �� ��ü�� �����Ѵ�.
	if(m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
	}

	// ī�޶� ��ü�� �����մϴ�.
	if(m_Camera)
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
	// �׷��� ����� �������մϴ�.
	return Render();
}


bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMMATRIX viewMatrix2, projectionMatrix2;
	
	// ���� �׸��� ���� ���۸� ����ϴ�
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// �� ����Ʈ ��ü���� �� �� ���� ����� �����ɴϴ�.
	m_ViewPoint->GetViewMatrix(viewMatrix2);
	m_ViewPoint->GetProjectionMatrix(projectionMatrix2);

	// ground �𵨿� ���� ������ �����մϴ�.
	worldMatrix = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	
	// ���� ���̴��� ����Ͽ� ���� ���� �������մϴ�.
	m_GroundModel->Render(m_Direct3D->GetDeviceContext());
	
	if(!m_ProjectionShader->Render(m_Direct3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_GroundModel->GetTexture(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetPosition(),
		viewMatrix2, projectionMatrix2, m_ProjectionTexture->GetTexture()))
	{
		return false;
	}

	// ���� ����� �缳���ϰ� ť�� �𵨿� ���� ��ȯ�� �����մϴ�.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixTranslation(0.0f, 2.0f, 0.0f);

	// �������� ���̴��� ����Ͽ� ť�� ���� �������մϴ�.
	m_CubeModel->Render(m_Direct3D->GetDeviceContext());

	if(!m_ProjectionShader->Render(m_Direct3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetPosition(),
		viewMatrix2, projectionMatrix2, m_ProjectionTexture->GetTexture()))
	{
		return false;
	}
							   
	// ������ �� ����� ȭ�鿡 ǥ���մϴ�.
	m_Direct3D->EndScene();

	return true;
}