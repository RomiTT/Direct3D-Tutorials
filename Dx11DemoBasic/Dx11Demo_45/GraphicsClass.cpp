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
	// Direct3D 객체 생성
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	bool result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// 셰이더 관리자 객체를 만듭니다.
	m_ShaderManager = new ShaderManagerClass;
	if(!m_ShaderManager)
	{
		return false;
	}

	// 셰이더 관리자 객체를 초기화합니다.
	result = m_ShaderManager->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	// 카메라 객체를 만듭니다.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// 카메라의 초기 위치를 설정합니다.
	m_Camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -10.0f));
	
	// light 객체를 만듭니다.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// 조명 객체를 초기화합니다.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(64.0f);

	// 모델 객체를 만듭니다.
	m_Model1 = new ModelClass;
	if(!m_Model1)
	{
		return false;
	}

	// 모델 객체를 초기화합니다.
	result = m_Model1->Initialize(m_D3D->GetDevice(), "../Dx11Demo_45/data/cube.txt", L"../Dx11Demo_45/data/marble.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the first model object.", L"Error", MB_OK);
		return false;
	}

	// 두 번째 모델 객체를 만듭니다.
	m_Model2 = new ModelClass;
	if(!m_Model2)
	{
		return false;
	}

	// 두 번째 모델 객체를 초기화합니다.
	result = m_Model2->Initialize(m_D3D->GetDevice(), "../Dx11Demo_45/data/cube.txt", L"../Dx11Demo_45/data/metal.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the second model object.", L"Error", MB_OK);
		return false;
	}

	// 법선 맵과 관련 벡터가있는 모델에 대해 세 번째 범프 모델 객체를 만듭니다.
	m_Model3 = new BumpModelClass;
	if(!m_Model3)
	{
		return false;
	}

	// 범프 모델 객체를 초기화합니다.
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
	// 모델 객체를 해제합니다.
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

	// light 오브젝트를 해제한다.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// 카메라 객체를 해제합니다.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// 셰이더 관리자 객체를 해제합니다.
	if(m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// D3D 객체를 해제합니다.
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

	// 각 프레임의 rotation 변수를 업데이트합니다.
	rotation += (float)XM_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// 그래픽 장면을 렌더링합니다.
	return Render(rotation);
}


bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	
	// 씬을 그리기 위해 버퍼를 지웁니다
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// 첫 번째 모델의 회전 및 평행 이동을 설정합니다.
	worldMatrix = XMMatrixRotationY(rotation);
	translateMatrix = XMMatrixTranslation(-3.5f, 0.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	// 텍스처 셰이더를 사용하여 첫 번째 모델을 렌더링합니다.
	m_Model1->Render(m_D3D->GetDeviceContext());

	if(!m_ShaderManager->RenderTextureShader(m_D3D->GetDeviceContext(), m_Model1->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model1->GetTexture()))
	{
		return false;
	}

	// 두 번째 모델의 회전 및 평행 이동을 설정합니다.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixRotationY(rotation);
	translateMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	// 라이트 쉐이더를 사용하여 두 번째 모델을 렌더링합니다.
	m_Model2->Render(m_D3D->GetDeviceContext());
	
	if(!m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model2->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower()))
	{
		return false;
	}

	// 셋째 모델의 회전 및 평행 이동을 설정합니다.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixRotationY(rotation);
	translateMatrix = XMMatrixTranslation(3.5f, 0.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	// 범프 맵 셰이더를 사용하여 세 번째 모델을 렌더링합니다.
	m_Model3->Render(m_D3D->GetDeviceContext());
	
	if(!m_ShaderManager->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_Model3->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model3->GetColorTexture(), m_Model3->GetNormalMapTexture(), m_Light->GetDirection(),
		m_Light->GetDiffuseColor()))
	{
		return false;
	}
							   
	// 렌더링 된 장면을 화면에 표시합니다.
	m_D3D->EndScene();

	return true;
}