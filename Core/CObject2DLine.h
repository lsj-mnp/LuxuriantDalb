#pragma once
#include "SharedHeader.h"

struct SVertex2DLine
{
	SVertex2DLine() {}
	SVertex2DLine(DirectX::XMVECTOR _Position, DirectX::XMVECTOR _Color) : Position{ _Position }, Color{ _Color }{}

	DirectX::XMVECTOR Position{};
	DirectX::XMVECTOR Color{};
};

struct S2DLine
{
	SVertex2DLine VertexA{};
	SVertex2DLine VertexB{};
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

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer{};

private:
	std::vector<S2DLine> m_vLines{};
};
