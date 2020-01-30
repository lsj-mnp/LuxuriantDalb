#pragma once

#include "SharedHeader.h"

class CShader
{
public:
	CShader(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
		m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
	{
		assert(PtrDevice);
		assert(PtrDeviceContext);
	}
	~CShader() {}

public:
	void Create(EShaderType eShaderType, const std::wstring& FileName, const std::string& EntryPoint,
		const D3D11_INPUT_ELEMENT_DESC* const InputElements = nullptr, size_t InputElementCount = 0);

	void Use();

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_Blob{};
	Microsoft::WRL::ComPtr<ID3D11DeviceChild> m_VShader{};
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout{};

	EShaderType m_eShaderType{};
};