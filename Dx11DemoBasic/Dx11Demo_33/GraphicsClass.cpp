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
	// Direct3D 객체 생성
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
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

	// 모델 객체 생성
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// 모델 객체 초기화
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), "../Dx11Demo_33/data/square.txt", L"../Dx11Demo_33/data/fire01.dds", 
								 L"../Dx11Demo_33/data/noise01.dds", L"../Dx11Demo_33/data/alpha01.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// 파이어 쉐이더 객체를 생성한다.
	m_FireShader = new FireShaderClass;
	if(!m_FireShader)
	{
		return false;
	}

	// 화재 쉐이더 객체를 초기화합니다.
	if(!m_FireShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the fire shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// fire shader 객체를 해제한다.
	if (m_FireShader)
	{
		m_FireShader->Shutdown();
		delete m_FireShader;
		m_FireShader = 0;
	}

	// 모델 객체를 해제합니다.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// m_Camera 객체 반환
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D 객체 반환
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}


bool GraphicsClass::Frame()
{
	// 카메라 위치를 설정합니다.
	m_Camera->SetPosition(0.0f, 0.0f, -3.0f);

	// 장면을 렌더링합니다.
	return Render();
}


bool GraphicsClass::Render()
{
	static float frameTime = 0.0f;


	// 프레임 시간 카운터를 증가시킵니다.
	frameTime += 0.01f;
	if(frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}

	// 세 가지 다른 노이즈 텍스처에 대해 세 가지 스크롤 속도를 설정합니다.
	XMFLOAT3 scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);

	// 세 개의 서로 다른 노이즈 옥타브 텍스처를 만드는 데 사용할 세 개의 스케일을 설정합니다.
	XMFLOAT3 scales = XMFLOAT3(1.0f, 2.0f, 3.0f);

	// 세 가지 다른 노이즈 텍스처에 대해 세 가지 다른 x 및 y 왜곡 인수를 설정합니다.
	XMFLOAT2 distortion1 = XMFLOAT2(0.1f, 0.2f);
	XMFLOAT2 distortion2 = XMFLOAT2(0.1f, 0.3f);
	XMFLOAT2 distortion3 = XMFLOAT2(0.1f, 0.1f);

	// 텍스처 좌표 샘플링 섭동의 스케일과 바이어스.
	float distortionScale = 0.8f;
	float distortionBias = 0.5f;

	// 장면을 시작할 버퍼를 지운다.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 오쏘 (ortho) 행렬을 가져옵니다.
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// 화재 투명도의 알파 블렌드를 켭니다.
	m_Direct3D->TurnOnAlphaBlending();

	// 정사각형 모델의 정점과 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// 화재 쉐이더를 사용하여 사각형 모델을 렌더링합니다.
	if(!m_FireShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
							 m_Model->GetTexture1(), m_Model->GetTexture2(), m_Model->GetTexture3(), frameTime, scrollSpeeds,
							 scales, distortion1, distortion2, distortion3, distortionScale, distortionBias))
	{
		return false;
	}

	// 알파 블렌딩을 끕니다.
	m_Direct3D->TurnOffAlphaBlending();

	// 렌더링 된 장면을 화면에 표시합니다.
	m_Direct3D->EndScene();

	return true;
}