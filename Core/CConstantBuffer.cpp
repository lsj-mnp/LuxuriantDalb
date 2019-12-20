#include "CConstantBuffer.h"

void CConstantBuffer::Create(EShaderType eShaderType, UINT ByteWidth, const void* const pData)
{
	m_eShadertype = eShaderType;

	m_BufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	m_BufferDesc.ByteWidth = ByteWidth;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_SubresourceData.pSysMem = pData;

	m_PtrDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, m_ConstantBuffer.ReleaseAndGetAddressOf());
}

void CConstantBuffer::Update()
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};

	if (SUCCEEDED(m_PtrDeviceContext->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
	{
		memcpy(mapped_subresource.pData, m_SubresourceData.pSysMem, m_BufferDesc.ByteWidth);

		m_PtrDeviceContext->Unmap(m_ConstantBuffer.Get(), 0);
	}
}

void CConstantBuffer::Use()
{
	switch (m_eShadertype)
	{
	case EShaderType::VertexShader:
		m_PtrDeviceContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
		break;
	case EShaderType::PixelShader:
		m_PtrDeviceContext->PSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
		break;
	default:
		break;
	}
}

