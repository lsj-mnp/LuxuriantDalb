#pragma once
#include "SharedHeader.h"

class CShader;
class CConstantBuffer;

struct SVertex2DLine
{
	SVertex2DLine() {}
	SVertex2DLine(const DirectX::XMVECTOR& _Position, const DirectX::XMVECTOR& _Color) : Position{ _Position }, Color{ _Color }{}

	DirectX::XMVECTOR Position{};
	DirectX::XMVECTOR Color{};
};

struct S2DLine
{
	S2DLine(){}
	S2DLine(const SVertex2DLine& A, const SVertex2DLine& B) : VertexA{ A }, VertexB{ B }{}

	SVertex2DLine VertexA{};
	SVertex2DLine VertexB{};
};

struct SComponentTransform
{
	DirectX::XMVECTOR Translation{ 0, 0, 0, 1 };
	DirectX::XMVECTOR Rotation{};
	DirectX::XMVECTOR Scaling{ 1, 1, 1, 0 };
};

struct SCBSpaceData
{
	DirectX::XMMATRIX WorldMatrix{};
	DirectX::XMMATRIX ProjectionMatrix{};
	DirectX::XMMATRIX WorldProjection{};
};

class CObject2DLine
{
public:
	static constexpr D3D11_INPUT_ELEMENT_DESC KInputElementDesc[]
	{
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

public:
	CObject2DLine(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext);
	~CObject2DLine();

public:
	void Create(const std::vector<S2DLine>& vLines);

	void Draw(const DirectX::XMMATRIX& ProjectionMatrix);

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer{};

	UINT m_VStride{ sizeof(SVertex2DLine) };
	UINT m_VOffset{};

private:
	std::vector<S2DLine> m_vLines{};

private:
	std::unique_ptr<CShader> m_VS{};
	std::unique_ptr<CShader> m_PS{};

private:
	std::unique_ptr<CConstantBuffer> m_CBSpace{};

private:
	SCBSpaceData m_CBSpaceData{};

private:
	SComponentTransform m_ComponentTransform{};
};
