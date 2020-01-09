#include "CGame.h"

using std::string;
using std::make_unique;

void CGame::Create(HINSTANCE hInstance, WNDPROC WndProc, const string& WindowName)
{
	m_hInstance = hInstance;
	m_WndProc = WndProc;

	CreateWin32Window(WindowName);

	InitializeDirectX();

	m_VSDefault = std::make_unique<CShader>(m_Device.Get(), m_DeviceContext.Get());
	m_VSDefault->Create(EShaderType::VertexShader, L"Shader/VSDefault.hlsl", "main", 
		CObject2D::KInputElementDesc, ARRAYSIZE(CObject2D::KInputElementDesc));

	m_VSDefault_Instanced = std::make_unique<CShader>(m_Device.Get(), m_DeviceContext.Get());
	m_VSDefault_Instanced->Create(EShaderType::VertexShader, L"Shader/VSDefault.hlsl", "Instanced",
		CObject2D::KInputElementDesc, ARRAYSIZE(CObject2D::KInputElementDesc));

	m_PSDefault = std::make_unique<CShader>(m_Device.Get(), m_DeviceContext.Get());
	m_PSDefault->Create(EShaderType::PixelShader, L"Shader/PSDefault.hlsl", "main");
	
	m_PSDefault_Colorize = std::make_unique<CShader>(m_Device.Get(), m_DeviceContext.Get());
	m_PSDefault_Colorize->Create(EShaderType::PixelShader, L"Shader/PSDefault.hlsl", "Colorize");

	m_PlayerBullet = make_unique<CBullet>(m_Device.Get(), m_DeviceContext.Get());
	m_PlayerBullet->Create("Asset/bullet.png");
}


void CGame::CreateWin32Window(const std::string& WindowName)
{
	WNDCLASSEX wnd_cls{};
	wnd_cls.cbClsExtra = 0;
	wnd_cls.cbSize = sizeof(WNDCLASSEX);
	wnd_cls.cbWndExtra = 0;
	wnd_cls.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wnd_cls.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd_cls.hIcon = wnd_cls.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wnd_cls.hInstance = m_hInstance;
	wnd_cls.lpfnWndProc = m_WndProc;
	wnd_cls.lpszClassName = "Base";
	wnd_cls.lpszMenuName = nullptr;
	wnd_cls.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wnd_cls);

	m_hWnd = CreateWindowEx(0, "Base", WindowName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		static_cast<int>(m_Width), static_cast<int>(m_Height), nullptr, nullptr, m_hInstance, nullptr);

	ShowWindow(m_hWnd, SW_SHOW);

	UpdateWindow(m_hWnd);
}

void CGame::InitializeDirectX()
{
	m_ProjectionMatrix = DirectX::XMMatrixOrthographicLH(m_Width, m_Height, 0.0f, 1.0f);

	CreateSwapChain();

	CreateAndSetRenderTargetView();

	CreateViewports();

	CreateSamplers();

	CreateBlendStates();
}

void CGame::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.Height = (UINT)m_Height;
	swap_chain_desc.BufferDesc.Width = (UINT)m_Width;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.Flags = 0;
	swap_chain_desc.OutputWindow = m_hWnd;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_desc,
		m_SwapChain.ReleaseAndGetAddressOf(), m_Device.ReleaseAndGetAddressOf(), nullptr, m_DeviceContext.ReleaseAndGetAddressOf());
}

void CGame::CreateAndSetRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer{};

	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)back_buffer.ReleaseAndGetAddressOf());

	m_Device->CreateRenderTargetView(back_buffer.Get(), nullptr, m_RenderTargetView.ReleaseAndGetAddressOf());

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}

void CGame::CreateViewports()
{
	m_Viewport.Height = m_Height;
	m_Viewport.Width = m_Width;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;
}

void CGame::SetViewport()
{
	m_DeviceContext->RSSetViewports(1, &m_Viewport);
}

void CGame::CreateSamplers()
{
	D3D11_SAMPLER_DESC sampler_desc{};
	sampler_desc.AddressU = sampler_desc.AddressV = sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.BorderColor[0] = 0.0f;
	sampler_desc.BorderColor[1] = 0.0f;
	sampler_desc.BorderColor[2] = 0.0f;
	sampler_desc.BorderColor[3] = 1.0f;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	sampler_desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.MaxLOD = 0.0f;
	sampler_desc.MinLOD = D3D11_FLOAT32_MAX;
	sampler_desc.MipLODBias = 0.0f;

	m_Device->CreateSamplerState(&sampler_desc, m_SamplerStateLinearWrap.ReleaseAndGetAddressOf());
}

void CGame::CreateBlendStates()
{
	D3D11_BLEND_DESC blend_desc{};
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = TRUE;
	blend_desc.RenderTarget[0].BlendOp = blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Device->CreateBlendState(&blend_desc, m_BlendStateAlpha.ReleaseAndGetAddressOf());
}

bool CGame::AddObject2D(const std::string& Name, const DirectX::XMFLOAT2& Size, const std::string& TextureFileName)
{
	if (m_UMapObject2DNameToIndex.find(Name) != m_UMapObject2DNameToIndex.end())
	{
		MessageBox(nullptr, "Object2D의 Name이 Collision합니다.", "Collision", MB_OK | MB_ICONERROR);

		return false;
	}

	m_vObject2Ds.emplace_back(std::make_unique<CObject2D>(m_Device.Get(), m_DeviceContext.Get()));

	m_vObject2Ds.back()->Create(Name, Size, TextureFileName);

	m_UMapObject2DNameToIndex[Name] = m_vObject2Ds.size() - 1;

	return true;
}

bool CGame::AddObject2D(const std::string& Name, const std::string& TextureFileName)
{
	if (m_UMapObject2DNameToIndex.find(Name) != m_UMapObject2DNameToIndex.end())
	{
		MessageBox(nullptr, "Object2D의 Name이 Collision합니다.", "Collision", MB_OK | MB_ICONERROR);

		return false;
	}

	m_vObject2Ds.emplace_back(std::make_unique<CObject2D>(m_Device.Get(), m_DeviceContext.Get()));

	m_vObject2Ds.back()->CreateAsTextureSize(Name, TextureFileName);

	m_UMapObject2DNameToIndex[Name] = m_vObject2Ds.size() - 1;

	return true;
}

void CGame::SetPlayerObject2D(const std::string& Name)
{
	m_PlayerObject2DIndex = m_UMapObject2DNameToIndex.at(Name);

	m_PlayerBullet->Link(GetPlayerObject2D());
}

CObject2D* CGame::GetPlayerObject2D() const
{
	return m_vObject2Ds[m_PlayerObject2DIndex].get();
}

void CGame::CreateStarPool()
{
	m_StarPool = std::make_unique<CStarPool>(m_Device.Get(), m_DeviceContext.Get());

	m_StarPool->Create(m_VSDefault_Instanced.get(), m_PSDefault.get(), m_PSDefault_Colorize.get(), 200, 5, 3, 2);
}

void CGame::CreateEnemyPool()
{
	m_EnemyPool = std::make_unique<CEnemyPool>(m_Device.Get(), m_DeviceContext.Get());

	m_EnemyPool->Create(m_VSDefault_Instanced.get(), m_PSDefault.get(), &m_PhysicsEngine);

	SEnemyTypeData EnemySlave{};
	EnemySlave.Damage = 10.0f;
	EnemySlave.HitPoint = 10.0f;
	EnemySlave.Speed = 100.0f;

	m_EnemyPool->AddEnemyType("EnemySlave", "Asset/enemy_slave.png", EnemySlave);

	m_EnemyPool->SpawnEnemy("EnemySlave", DirectX::XMVectorSet(660.0f, 260.0f, 0, 1), DirectX::XMVectorSet(-1.0f, 0, 0, 1));
}

CObject2D* CGame::GetObject2DPtr(const std::string& Name) const
{
	size_t index{ m_UMapObject2DNameToIndex.at(Name) };

	return m_vObject2Ds[index].get();
}

CShader* CGame::GetShader(EShader eShader)
{
	switch (eShader)
	{
	case CGame::EShader::VSDefault:
		return m_VSDefault.get();
	case CGame::EShader::VSDefault_Instanced:
		return m_VSDefault_Instanced.get();
	case CGame::EShader::PSDefault:
		return m_PSDefault.get();
	case CGame::EShader::PSDefault_Colorize:
		return m_PSDefault_Colorize.get();
	default:
		break;
	}

	return nullptr;
}

void CGame::SpawnPlayerBullet()
{
	m_PlayerBullet->Spawn(700.0f);
}

void CGame::Update()
{
	m_CurrTime = m_Clock.now().time_since_epoch().count();
	if (m_PrevTime == 0) m_PrevTime = m_CurrTime;
	m_DeltaTime_ns = m_CurrTime - m_PrevTime;
	m_DeltaTime_us = (float)((double)(m_DeltaTime_ns) / 1000.0);
	m_DeltaTime_ms = (float)((double)(m_DeltaTime_us) / 1000.0);
	m_DeltaTime_s = (float)((double)(m_DeltaTime_ms) / 1000.0);

	m_PrevTime = m_CurrTime;

	m_PlayerBullet->Update(GetDeltaTime(), DirectX::XMFLOAT2(m_Width, m_Height));

	m_PhysicsEngine.Execute(GetDeltaTime());
}

void CGame::BeginRendering(const float* ColorRGBA)
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), ColorRGBA);
}

void CGame::Draw()
{
	m_DeviceContext->PSSetSamplers(0, 1, m_SamplerStateLinearWrap.GetAddressOf());

	m_DeviceContext->OMSetBlendState(m_BlendStateAlpha.Get(), nullptr, 0xFFFFFFFF);

	SetViewport();

	{
		for (int iObject2D = 0; iObject2D < (int)m_vObject2Ds.size(); ++iObject2D)
		{
			if (iObject2D == m_PlayerObject2DIndex) continue;
			CObject2D* const Object2D{ m_vObject2Ds[iObject2D].get() };
			DrawObject2D(Object2D);
		}

		m_StarPool->Update(GetDeltaTime());

		m_StarPool->Draw(m_ProjectionMatrix);

		m_EnemyPool->Draw(m_ProjectionMatrix);

		m_VSDefault->Use();

		m_PlayerBullet->Draw(m_ProjectionMatrix);

		CObject2D* const PlayerObject2D{ m_vObject2Ds[m_PlayerObject2DIndex].get() };
		DrawObject2D(PlayerObject2D);
	}
}

void CGame::EndRendering()
{
	m_SwapChain->Present(0, 0);
}

void CGame::DrawObject2D(CObject2D* const Object2D)
{
	if (Object2D->HasInstances())
	{
		m_VSDefault_Instanced->Use();
	}
	else
	{
		m_VSDefault->Use();
	}

	if (Object2D->GetPixelShader())
	{
		Object2D->GetPixelShader()->Use();
	}
	else
	{
		m_PSDefault->Use();
	}

	Object2D->Draw(m_ProjectionMatrix);
}

DirectX::Keyboard::State CGame::GetKeyboardState() const
{
	return m_Keyboard.GetState();
}

float CGame::GetDeltaTime() const
{
	return m_DeltaTime_s;
}

float CGame::GetWidth() const
{
	return m_Width;
}

float CGame::GetHeight() const
{
	return m_Height;
}

