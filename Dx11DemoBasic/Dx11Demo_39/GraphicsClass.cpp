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
	// Direct3D 객체 생성
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	if(!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
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

	// 카메라 포지션 설정
	m_Camera->SetPosition(0.0f, -1.5f, -7.0f);
	
	// 파티클 셰이더 개체를 만듭니다.
	m_ParticleShader = new ParticleShaderClass;
	if(!m_ParticleShader)
	{
		return false;
	}

	// 파티클 셰이더 개체를 초기화합니다.
	if(!m_ParticleShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// 파티클 시스템 객체를 만듭니다.
	m_ParticleSystem = new ParticleSystemClass;
	if(!m_ParticleSystem)
	{
		return false;
	}

	// 파티클 시스템 객체를 초기화합니다.
	if(!m_ParticleSystem->Initialize(m_Direct3D->GetDevice(), L"../Dx11Demo_39/data/star.dds"))
	{
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// 파티클 시스템 객체를 해제합니다.
	if(m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// particle shader 객체를 해제한다.
	if(m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	// m_Camera 객체 반환
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D 객체 반환
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}


bool GraphicsClass::Frame(float frameTime)
{
	// 파티클 시스템에 대한 프레임 처리를 실행합니다.
	m_ParticleSystem->Frame(frameTime, m_Direct3D->GetDeviceContext());

	// 그래픽 랜더링 처리
	return Render();
}


bool GraphicsClass::Render()
{
	// 씬을 그리기 위해 버퍼를 지웁니다
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// 알파 블렌딩을 켭니다.
	m_Direct3D->EnableAlphaBlending();

	// 파티클 시스템 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	m_ParticleSystem->Render(m_Direct3D->GetDeviceContext());

	// 텍스처 셰이더를 사용하여 모델을 렌더링합니다.
	if(!m_ParticleShader->Render(m_Direct3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
									  m_ParticleSystem->GetTexture()))
	{
		return false;
	}

	// 알파 블렌딩을 끕니다.
	m_Direct3D->DisableAlphaBlending();

	// 렌더링 된 장면을 화면에 표시합니다.
	m_Direct3D->EndScene();

	return true;
}