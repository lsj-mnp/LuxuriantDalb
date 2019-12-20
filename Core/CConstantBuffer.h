#pragma once

#include "SharedHeader.h"

class CConstantBuffer
{
public:
	CConstantBuffer(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
		m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
	{
		assert(PtrDevice);
		assert(PtrDeviceContext);
	}
	~CConstantBuffer() {}

public:
	void Create(EShaderType eShaderType, UINT ByteWidth, const void* const pData);

	void Update();

	void Use();

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer{};
	D3D11_SUBRESOURCE_DATA m_SubresourceData{};
	D3D11_BUFFER_DESC m_BufferDesc{};
	EShaderType m_eShadertype{};
};
