#include "CShader.h"

void CShader::Create(EShaderType eShaderType, const std::wstring& FileName, const std::string& EntryPoint,
	const D3D11_INPUT_ELEMENT_DESC* const InputElements, size_t InputElementCount)
{
	m_eShaderType = eShaderType;

	switch (m_eShaderType)
	{
	case EShaderType::VertexShader:
		D3DCompileFromFile(FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint.c_str(), "vs_4_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_Blob.ReleaseAndGetAddressOf(), nullptr);

		m_PtrDevice->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr,
			(ID3D11VertexShader**)m_VShader.ReleaseAndGetAddressOf());

		if (InputElements && InputElementCount)
		{
			m_PtrDevice->CreateInputLayout(InputElements, InputElementCount, m_Blob->GetBufferPointer(),
				m_Blob->GetBufferSize(), m_InputLayout.ReleaseAndGetAddressOf());
		}
		break;
	case EShaderType::PixelShader:
		D3DCompileFromFile(FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint.c_str(), "ps_4_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_Blob.ReleaseAndGetAddressOf(), nullptr);

		m_PtrDevice->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, 
			(ID3D11PixelShader**)m_VShader.ReleaseAndGetAddressOf());
		break;
	default:
		break;
	}
}

void CShader::Use()
{
	switch (m_eShaderType)
	{
	case EShaderType::VertexShader:
		m_PtrDeviceContext->VSSetShader((ID3D11VertexShader*)m_VShader.Get(), nullptr, 0);

		m_PtrDeviceContext->IASetInputLayout(m_InputLayout.Get());

		break;
	case EShaderType::PixelShader:
		m_PtrDeviceContext->PSSetShader((ID3D11PixelShader*)m_VShader.Get(), nullptr, 0);

		break;
	default:
		break;
	}
}