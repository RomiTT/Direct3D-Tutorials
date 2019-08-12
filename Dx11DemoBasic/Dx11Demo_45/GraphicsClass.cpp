#include "stdafx.h"
#include "d3dclass.h"
#include "shadermanagerclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "modelclass.h"
#include "bumpmodelclass.h"
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
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Direct3D ��ü �ʱ�ȭ
	bool result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// ���̴� ������ ��ü�� ����ϴ�.
	m_ShaderManager = new ShaderManagerClass;
	if(!m_ShaderManager)
	{
		return false;
	}

	// ���̴� ������ ��ü�� �ʱ�ȭ�մϴ�.
	result = m_ShaderManager->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	// ī�޶� ��ü�� ����ϴ�.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// ī�޶��� �ʱ� ��ġ�� �����մϴ�.
	m_Camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -10.0f));
	
	// light ��ü�� ����ϴ�.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// ���� ��ü�� �ʱ�ȭ�մϴ�.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(64.0f);

	// �� ��ü�� ����ϴ�.
	m_Model1 = new ModelClass;
	if(!m_Model1)
	{
		return false;
	}

	// �� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_Model1->Initialize(m_D3D->GetDevice(), "../Dx11Demo_45/data/cube.txt", L"../Dx11Demo_45/data/marble.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the first model object.", L"Error", MB_OK);
		return false;
	}

	// �� ��° �� ��ü�� ����ϴ�.
	m_Model2 = new ModelClass;
	if(!m_Model2)
	{
		return false;
	}

	// �� ��° �� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_Model2->Initialize(m_D3D->GetDevice(), "../Dx11Demo_45/data/cube.txt", L"../Dx11Demo_45/data/metal.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the second model object.", L"Error", MB_OK);
		return false;
	}

	// ���� �ʰ� ���� ���Ͱ��ִ� �𵨿� ���� �� ��° ���� �� ��ü�� ����ϴ�.
	m_Model3 = new BumpModelClass;
	if(!m_Model3)
	{
		return false;
	}

	// ���� �� ��ü�� �ʱ�ȭ�մϴ�.
	result = m_Model3->Initialize(m_D3D->GetDevice(), "../Dx11Demo_45/data/cube.txt", L"../Dx11Demo_45/data/stone.dds", 
								  L"../Dx11Demo_45/data/normal.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the third model object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// �� ��ü�� �����մϴ�.
	if(m_Model1)
	{
		m_Model1->Shutdown();
		delete m_Model1;
		m_Model1 = 0;
	}

	if(m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	if(m_Model3)
	{
		m_Model3->Shutdown();
		delete m_Model3;
		m_Model3 = 0;
	}

	// light ������Ʈ�� �����Ѵ�.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// ī�޶� ��ü�� �����մϴ�.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// ���̴� ������ ��ü�� �����մϴ�.
	if(m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// D3D ��ü�� �����մϴ�.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
}


bool GraphicsClass::Frame()
{
	static float rotation = 0.0f;

	// �� �������� rotation ������ ������Ʈ�մϴ�.
	rotation += (float)XM_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// �׷��� ����� �������մϴ�.
	return Render(rotation);
}


bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	
	// ���� �׸��� ���� ���۸� ����ϴ�
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// ù ��° ���� ȸ�� �� ���� �̵��� �����մϴ�.
	worldMatrix = XMMatrixRotationY(rotation);
	translateMatrix = XMMatrixTranslation(-3.5f, 0.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	// �ؽ�ó ���̴��� ����Ͽ� ù ��° ���� �������մϴ�.
	m_Model1->Render(m_D3D->GetDeviceContext());

	if(!m_ShaderManager->RenderTextureShader(m_D3D->GetDeviceContext(), m_Model1->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model1->GetTexture()))
	{
		return false;
	}

	// �� ��° ���� ȸ�� �� ���� �̵��� �����մϴ�.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixRotationY(rotation);
	translateMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	// ����Ʈ ���̴��� ����Ͽ� �� ��° ���� �������մϴ�.
	m_Model2->Render(m_D3D->GetDeviceContext());
	
	if(!m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model2->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower()))
	{
		return false;
	}

	// ��° ���� ȸ�� �� ���� �̵��� �����մϴ�.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixRotationY(rotation);
	translateMatrix = XMMatrixTranslation(3.5f, 0.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	// ���� �� ���̴��� ����Ͽ� �� ��° ���� �������մϴ�.
	m_Model3->Render(m_D3D->GetDeviceContext());
	
	if(!m_ShaderManager->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_Model3->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model3->GetColorTexture(), m_Model3->GetNormalMapTexture(), m_Light->GetDirection(),
		m_Light->GetDiffuseColor()))
	{
		return false;
	}
							   
	// ������ �� ����� ȭ�鿡 ǥ���մϴ�.
	m_D3D->EndScene();

	return true;
}