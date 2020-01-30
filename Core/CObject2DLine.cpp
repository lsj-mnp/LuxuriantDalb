#include "CObject2DLine.h"
#include "CShader.h"
#include "CConstantBuffer.h"

CObject2DLine::CObject2DLine(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
	m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
{
	assert(PtrDevice);
	assert(PtrDeviceContext);
}

CObject2DLine::~CObject2DLine()
{
}

void CObject2DLine::Create(const std::vector<S2DLine>& vLines)
{
	assert(!vLines.empty());

	m_vLines = vLines;

	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.ByteWidth = sizeof(S2DLine) * vLines.size();
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_vLines[0];

		m_PtrDevice->CreateBuffer(&buffer_desc, &subresource_data, m_VertexBuffer.ReleaseAndGetAddressOf());
	}

	m_VS = std::make_unique<CShader>(m_PtrDevice, m_PtrDeviceContext);
	m_VS->Create(EShaderType::VertexShader, L"Shader/VSLine.hlsl", "main", KInputElementDesc, ARRAYSIZE(KInputElementDesc));

	m_PS = std::make_unique<CShader>(m_PtrDevice, m_PtrDeviceContext);
	m_PS->Create(EShaderType::PixelShader, L"Shader/PSLine.hlsl", "main");

	m_CBSpace = std::make_unique<CConstantBuffer>(m_PtrDevice, m_PtrDeviceContext);
	m_CBSpace->Create(EShaderType::VertexShader, sizeof(m_CBSpaceData), &m_CBSpaceData);
}

void CObject2DLine::Draw(const DirectX::XMMATRIX& ProjectionMatrix)
{
	m_PtrDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VStride, &m_VOffset);

	m_PtrDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	m_VS->Use();
	m_PS->Use();

	DirectX::XMMATRIX Translation{ DirectX::XMMatrixTranslationFromVector(m_ComponentTransform.Translation) };
	DirectX::XMMATRIX Scaling{ DirectX::XMMatrixScalingFromVector(m_ComponentTransform.Scaling) };
	DirectX::XMMATRIX Rotation{ DirectX::XMMatrixRotationRollPitchYawFromVector(m_ComponentTransform.Rotation) };

	DirectX::XMMATRIX WorldMatrix{ Scaling * Rotation * Translation };

	m_CBSpaceData.WorldMatrix = DirectX::XMMatrixTranspose(WorldMatrix);
	m_CBSpaceData.ProjectionMatrix = DirectX::XMMatrixTranspose(ProjectionMatrix);
	m_CBSpaceData.WorldProjection = DirectX::XMMatrixTranspose(WorldMatrix * ProjectionMatrix);
		
	m_CBSpace->Update();
	m_CBSpace->Use();
	
	m_PtrDeviceContext->Draw(m_vLines.size() * 2, 0);
}
