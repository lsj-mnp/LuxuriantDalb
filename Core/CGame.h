#pragma once

#include "SharedHeader.h"
#include "CShader.h"
#include "CConstantBuffer.h"
#include "CObject2D.h"
#include "CStarPool.h"

class CGame
{
public:
	enum class EShader
	{
		VSDefault,
		VSDefault_Instanced,
		PSDefault,
		PSDefault_Colorize,
	};

public:
	CGame(float Width, float Height) : m_Width{ Width }, m_Height{ Height }{}
	~CGame() {}

public:
	void Create(HINSTANCE hInstance, WNDPROC WndProc, const std::string& WindowName);

private:
	void CreateWin32Window(const std::string& WindowName);

	void InitializeDirectX();

private:
	void CreateSwapChain();

	void CreateAndSetRenderTargetView();

	void CreateViewports();

	void SetViewport();

	void CreateSamplers();

	void CreateBlendStates();

public:
	bool AddObject2D(const std::string& Name, const DirectX::XMFLOAT2& Size, const std::string& TextureFileName);

	bool AddObject2D(const std::string& Name, const std::string& TextureFileName);

public:
	void SetPlayerObject2D(const std::string& Name);

	CObject2D* GetPlayerObject2D() const;

public:
	void CreateStarPool();

public:
	CObject2D* GetObject2DPtr(const std::string& Name) const;

public:
	CShader* GetShader(EShader eShader);

public:
	void BeginRendering(const float* ColorRGBA);

	void Draw();

	void EndRendering();

private:
	void DrawObject2D(CObject2D* const Object2D);

public:
	DirectX::Keyboard::State GetKeyboardState() const;

private:
	HINSTANCE m_hInstance{};
	HWND m_hWnd{};
	WNDPROC m_WndProc{};

	float m_Width{};
	float m_Height{};

private:
	std::unique_ptr<CShader> m_VSDefault{};
	std::unique_ptr<CShader> m_VSDefault_Instanced{};
	std::unique_ptr<CShader> m_PSDefault{};
	std::unique_ptr<CShader> m_PSDefault_Colorize{};

private:
	std::unique_ptr<CStarPool> m_StarPool{};

private:
	std::vector<std::unique_ptr<CObject2D>> m_vObject2Ds{};
	std::unordered_map<std::string, size_t> m_UMapObject2DNameToIndex{};

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain{};
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device{};
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext{};
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView{};

	D3D11_VIEWPORT m_Viewport{};

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerStateLinearWrap{};
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStateAlpha{};

private:
	DirectX::XMMATRIX m_ProjectionMatrix{};

private:
	DirectX::Keyboard m_Keyboard{};

private:
	int m_PlayerObject2DIndex{};
};