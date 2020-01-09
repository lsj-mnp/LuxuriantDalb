#include "CObject2DLine.h"

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
}
