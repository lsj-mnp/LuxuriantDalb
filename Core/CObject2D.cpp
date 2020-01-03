#include "CObject2D.h"
#include "CConstantBuffer.h"

using std::string;
using std::to_string;
using std::make_unique;

void CObject2D::CModel2D::CreateRectangle(const DirectX::XMFLOAT2& Size)
{
	m_Size = Size;

	float HalfSizeX{ m_Size.x * 0.5f };
	float HalfSizeY{ m_Size.y * 0.5f };

	DirectX::XMFLOAT2 Pos0{ -HalfSizeX, +HalfSizeY };
	DirectX::XMFLOAT2 Pos1{ +HalfSizeX, +HalfSizeY };
	DirectX::XMFLOAT2 Pos2{ -HalfSizeX, -HalfSizeY };
	DirectX::XMFLOAT2 Pos3{ +HalfSizeX, -HalfSizeY };

	m_vVertices.emplace_back(Pos0, DirectX::XMFLOAT2(0.0f, 0.0f));
	m_vVertices.emplace_back(Pos1, DirectX::XMFLOAT2(1.0f, 0.0f));
	m_vVertices.emplace_back(Pos2, DirectX::XMFLOAT2(0.0f, 1.0f));
	m_vVertices.emplace_back(Pos3, DirectX::XMFLOAT2(1.0f, 1.0f));

	m_vTriangles.emplace_back(0, 1, 2);
	m_vTriangles.emplace_back(1, 3, 2);
	
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.ByteWidth = sizeof(SVertex2D) * 6;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_vVertices[0];

		m_PtrDevice->CreateBuffer(&buffer_desc, &subresource_data, m_VertexBuffer.ReleaseAndGetAddressOf());
	}

	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		buffer_desc.ByteWidth = (UINT)(sizeof(STriangle) * m_vTriangles.size());
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_vTriangles[0];

		m_PtrDevice->CreateBuffer(&buffer_desc, &subresource_data, m_IndexBuffer.ReleaseAndGetAddressOf());
	}
}

void CObject2D::CModel2D::UpdateRectangleTexCoord(const DirectX::XMFLOAT2& UVOffset, const DirectX::XMFLOAT2& UVSize)
{
	m_vVertices[0].TexCoord = UVOffset;
	m_vVertices[1].TexCoord = DirectX::XMFLOAT2(UVOffset.x + UVSize.x	, UVOffset.y);
	m_vVertices[2].TexCoord = DirectX::XMFLOAT2(UVOffset.x				, UVOffset.y + UVSize.y);
	m_vVertices[3].TexCoord = DirectX::XMFLOAT2(UVOffset.x + UVSize.x	, UVOffset.y + UVSize.y);

	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	if (SUCCEEDED(m_PtrDeviceContext->Map(m_VertexBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
	{ 
		memcpy(mapped_subresource.pData, &m_vVertices[0], sizeof(SVertex2D) * m_vVertices.size());

		m_PtrDeviceContext->Unmap(m_VertexBuffer.Get(), 0);
	}
}

void CObject2D::CModel2D::SetInputBuffers()
{
	m_PtrDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VBStride, &m_VBOffset);

	m_PtrDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
}

const DirectX::XMFLOAT2& CObject2D::CModel2D::GetSize() const
{
	return m_Size;
}

UINT CObject2D::CModel2D::GetIndexCount() const
{
	return (UINT)(m_vTriangles.size() * 3);
}

CObject2D::CObject2D(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
	m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
{
	assert(PtrDevice);
	assert(PtrDeviceContext);
}

CObject2D::~CObject2D()
{
}

void CObject2D::Create(const std::string& Name, const DirectX::XMFLOAT2& Size, const std::string& TextureFileName)
{
	m_Name = Name;

	m_CBSpace = make_unique<CConstantBuffer>(m_PtrDevice, m_PtrDeviceContext);
	m_CBSpace->Create(EShaderType::VertexShader, sizeof(m_CBSpaceData), &m_CBSpaceData);

	m_Model2D = std::make_unique<CModel2D>(m_PtrDevice, m_PtrDeviceContext);
	m_Model2D->CreateRectangle(Size);

	m_Texture = std::make_unique<CTexture>(m_PtrDevice, m_PtrDeviceContext);
	m_Texture->CreateFromFile(TextureFileName);
	m_Texture->SetShaderType(EShaderType::PixelShader);
}

void CObject2D::CreateAsTextureSize(const std::string& Name, const std::string& TextureFileName)
{
	m_Name = Name;

	m_CBSpace = make_unique<CConstantBuffer>(m_PtrDevice, m_PtrDeviceContext);
	m_CBSpace->Create(EShaderType::VertexShader, sizeof(m_CBSpaceData), &m_CBSpaceData);

	m_Texture = std::make_unique<CTexture>(m_PtrDevice, m_PtrDeviceContext);
	m_Texture->CreateFromFile(TextureFileName);
	m_Texture->SetShaderType(EShaderType::PixelShader);
	D3D11_TEXTURE2D_DESC Texture2DDesc{ m_Texture->GetTextureDesc() };

	m_Model2D = std::make_unique<CModel2D>(m_PtrDevice, m_PtrDeviceContext);
	m_Model2D->CreateRectangle(DirectX::XMFLOAT2((float)Texture2DDesc.Width, (float)Texture2DDesc.Height));
}

void CObject2D::SetVisibleArea(const DirectX::XMFLOAT2& PixelSpaceOffset)
{
	D3D11_TEXTURE2D_DESC Texture2DDesc{ m_Texture->GetTextureDesc() };

	DirectX::XMFLOAT2 PixelSpaceTotalSize{ (float)Texture2DDesc.Width, (float)Texture2DDesc.Height };
	DirectX::XMFLOAT2 PixelSpaceSize{ m_Model2D->GetSize() };

	DirectX::XMFLOAT2 UVOffset{ PixelSpaceOffset.x / PixelSpaceTotalSize.x , PixelSpaceOffset.y / PixelSpaceTotalSize.y };
	DirectX::XMFLOAT2 UVSize{ PixelSpaceSize.x / PixelSpaceTotalSize.x, PixelSpaceSize.y / PixelSpaceTotalSize.y };

	m_Model2D->UpdateRectangleTexCoord(UVOffset, UVSize);
}

void CObject2D::SetPixelShader(CShader* const PixelShader)
{
	m_PtrPixelShader = PixelShader;
}

CShader* CObject2D::GetPixelShader()
{
	return m_PtrPixelShader;
}

void CObject2D::Draw(const DirectX::XMMATRIX& ProjectionMatrix)
{
	m_CBSpaceData.WorldMatrix = DirectX::XMMatrixTranspose(GetWorldMatrix());
	m_CBSpaceData.ProjectionMatrix = DirectX::XMMatrixTranspose(ProjectionMatrix);
	m_CBSpaceData.WorldProjection = DirectX::XMMatrixTranspose(GetWorldMatrix() * ProjectionMatrix);

	m_CBSpace->Update();
	m_CBSpace->Use();

	m_Texture->Use();

	m_Model2D->SetInputBuffers();

	m_PtrDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (HasInstances())
	{
		m_PtrDeviceContext->IASetVertexBuffers(1, 1, m_InstanceBuffer.GetAddressOf(), &m_IBStride, &m_IBOffset);

		m_PtrDeviceContext->DrawIndexedInstanced(m_Model2D->GetIndexCount(), (UINT)GetInstanceCount(), 0, 0, 0);
	}
	else
	{
		m_PtrDeviceContext->DrawIndexed(m_Model2D->GetIndexCount(), 0, 0);
	}
}

void CObject2D::Translate(const DirectX::XMVECTOR& Translation)
{
	using namespace DirectX;

	m_ComponentTransform.Translation += Translation;

	UpdateWorldMatrix();
}

void CObject2D::Rotate(const DirectX::XMVECTOR& Rotation)
{
	using namespace DirectX;

	m_ComponentTransform.Rotation += Rotation;

	UpdateWorldMatrix();
}

void CObject2D::Scale(const DirectX::XMVECTOR& Scaling)
{
	using namespace DirectX;

	m_ComponentTransform.Scaling += Scaling;

	UpdateWorldMatrix();
}

void CObject2D::TranslateTo(const DirectX::XMVECTOR& Translation)
{
	using namespace DirectX;

	m_ComponentTransform.Translation = Translation;

	UpdateWorldMatrix();
}

void CObject2D::RotateTo(const DirectX::XMVECTOR& Rotation)
{
	using namespace DirectX;

	m_ComponentTransform.Rotation = Rotation;

	UpdateWorldMatrix();
}

void CObject2D::ScaleTo(const DirectX::XMVECTOR& Scaling)
{
	using namespace DirectX;

	m_ComponentTransform.Scaling = Scaling;

	UpdateWorldMatrix();
}

const DirectX::XMMATRIX& CObject2D::GetWorldMatrix() const
{
	return m_ComponentTransform.WorldMatrix;
}

const DirectX::XMVECTOR& CObject2D::GetTranslation() const
{
	return m_ComponentTransform.Translation;
}

const DirectX::XMVECTOR& CObject2D::GetRotation() const
{
	return m_ComponentTransform.Rotation;
}

void CObject2D::UpdateWorldMatrix()
{
	DirectX::XMMATRIX Translation{ DirectX::XMMatrixTranslationFromVector(m_ComponentTransform.Translation) };
	DirectX::XMMATRIX Rotation{ DirectX::XMMatrixRotationRollPitchYawFromVector(m_ComponentTransform.Rotation) };
	DirectX::XMMATRIX Scaling{ DirectX::XMMatrixScalingFromVector(m_ComponentTransform.Scaling) };

	m_ComponentTransform.WorldMatrix = Scaling * Rotation * Translation;
}

bool CObject2D::InsertInstance(const std::string& Name)
{
	if (m_UMapInstanceNameToIndex.find(Name) != m_UMapInstanceNameToIndex.end())
	{
		MessageBox(nullptr, "Instance의 Name이 Collision합니다.", "Collision", MB_OK | MB_ICONERROR);

		return false;
	}

	bool bShouldRecreateBuffer{ m_vInstanceGPUData.size() == m_vInstanceGPUData.capacity() };

	m_vInstanceCPUData.emplace_back();

	m_vInstanceCPUData.back().Scaling = m_ComponentTransform.Scaling;

	m_vInstanceGPUData.emplace_back();

	m_vInstanceCPUData.back().Name = Name;

	m_UMapInstanceNameToIndex[Name] = m_vInstanceCPUData.size() - 1;

	UpdateInstanceWorldMatrix(Name);

	if (bShouldRecreateBuffer) CreateInstanceBuffer();

	return true;
}

bool CObject2D::InsertInstance()
{
	size_t instance_count{ GetInstanceCount() };

	string name{ "instance" + to_string(instance_count) };

	return InsertInstance(name);
}

void CObject2D::TranslateInstance(size_t InstanceID, const DirectX::XMVECTOR& Translation)
{
	using namespace DirectX;

	m_vInstanceCPUData[InstanceID].Translation += Translation;
	UpdateInstanceWorldMatrix(InstanceID);
}

void CObject2D::TranslateInstance(const std::string& Name, const DirectX::XMVECTOR& Translation)
{
	size_t index{ m_UMapInstanceNameToIndex.at(Name) };
	TranslateInstance(index, Translation);
}

void CObject2D::TranslateInstanceTo(size_t InstanceID, const DirectX::XMVECTOR& Translation)
{
	m_vInstanceCPUData[InstanceID].Translation = Translation;
	UpdateInstanceWorldMatrix(InstanceID);
}

void CObject2D::TranslateInstanceTo(const std::string& Name, const DirectX::XMVECTOR& Translation)
{
	size_t index{ m_UMapInstanceNameToIndex.at(Name) };
	TranslateInstanceTo(index, Translation);
}

void CObject2D::RotateInstance(size_t InstanceID, const DirectX::XMVECTOR& Rotation)
{
	using namespace DirectX;

	m_vInstanceCPUData[InstanceID].Rotation += Rotation;

	UpdateInstanceWorldMatrix(InstanceID);
}

void CObject2D::RotateInstance(const std::string& Name, const DirectX::XMVECTOR& Rotation)
{
	size_t index{ m_UMapInstanceNameToIndex.at(Name) };
	RotateInstance(index, Rotation);
}

void CObject2D::RotateInstanceTo(size_t InstanceID, const DirectX::XMVECTOR& Rotation)
{
	using namespace DirectX;

	m_vInstanceCPUData[InstanceID].Rotation = Rotation;

	UpdateInstanceWorldMatrix(InstanceID);
}

void CObject2D::RotateInstanceTo(const std::string& Name, const DirectX::XMVECTOR& Rotation)
{
	size_t index{ m_UMapInstanceNameToIndex.at(Name) };
	RotateInstanceTo(index, Rotation);
}

void CObject2D::ScaleInstance(size_t InstanceID, const DirectX::XMVECTOR& Scaling)
{
	using namespace DirectX;

	m_vInstanceCPUData[InstanceID].Scaling += Scaling;

	UpdateInstanceWorldMatrix(InstanceID);
}

void CObject2D::ScaleInstance(const std::string& Name, const DirectX::XMVECTOR& Scaling)
{
	size_t index{ m_UMapInstanceNameToIndex.at(Name) };
	ScaleInstance(index, Scaling);
}

void CObject2D::ScaleInstanceTo(size_t InstanceID, const DirectX::XMVECTOR& Scaling)
{
	using namespace DirectX;

	m_vInstanceCPUData[InstanceID].Scaling = Scaling;
	UpdateInstanceWorldMatrix(InstanceID);
}

void CObject2D::ScaleInstanceTo(const std::string& Name, const DirectX::XMVECTOR& Scaling)
{
	using namespace DirectX;

	size_t index{ m_UMapInstanceNameToIndex.at(Name) };
	ScaleInstanceTo(index, Scaling);
}

bool CObject2D::HasInstances() const
{
	return (m_vInstanceCPUData.size() > 0) ? true : false;
}

size_t CObject2D::GetInstanceCount() const
{
	return m_vInstanceCPUData.size();
}

const std::string& CObject2D::GetLastInstanceName() const
{
	return m_vInstanceCPUData.back().Name;
}

const DirectX::XMVECTOR& CObject2D::GetInstanceTranslation(size_t InstanceID) const
{
	return m_vInstanceCPUData[InstanceID].Translation;
}

const DirectX::XMVECTOR& CObject2D::GetInstanceTranslation(const std::string& Name) const
{
	return GetInstanceTranslation(m_UMapInstanceNameToIndex.at(Name));
}

const DirectX::XMVECTOR& CObject2D::GetInstanceScaling(size_t InstanceID) const
{
	return m_vInstanceCPUData[InstanceID].Scaling;
}

void CObject2D::CreateInstanceBuffer()
{
	if (m_vInstanceGPUData.empty()) return;

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.ByteWidth = sizeof(SInstanceGPUData) * m_vInstanceGPUData.capacity();
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA subresource_data{};
	subresource_data.pSysMem = &m_vInstanceGPUData[0];

	m_PtrDevice->CreateBuffer(&buffer_desc, &subresource_data, m_InstanceBuffer.ReleaseAndGetAddressOf());
}

void CObject2D::UpdateInstanceBuffer()
{
	if (!m_InstanceBuffer) return;

	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};

	if (SUCCEEDED(m_PtrDeviceContext->Map(m_InstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
	{
		memcpy(mapped_subresource.pData, &m_vInstanceGPUData[0], sizeof(SInstanceGPUData) * m_vInstanceGPUData.size());

		m_PtrDeviceContext->Unmap(m_InstanceBuffer.Get(), 0);
	}
}

void CObject2D::UpdateInstanceWorldMatrix(size_t InstanceID)
{
	m_vInstanceGPUData[InstanceID].WorldMatrix =
		DirectX::XMMatrixScalingFromVector(m_vInstanceCPUData[InstanceID].Scaling) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(m_vInstanceCPUData[InstanceID].Rotation) *
		DirectX::XMMatrixTranslationFromVector(m_vInstanceCPUData[InstanceID].Translation);
}

void CObject2D::UpdateInstanceWorldMatrix(const std::string& Name)
{
	size_t index{ m_UMapInstanceNameToIndex.at(Name) };

	UpdateInstanceWorldMatrix(index);
}

void CObject2D::CTexture::CreateFromFile(const std::string& FileName)
{
	std::wstring wFileName{ FileName.begin(), FileName.end() };

	DirectX::CreateWICTextureFromFile(m_PtrDevice, wFileName.c_str(), 
		(ID3D11Resource**)m_Texture2D.ReleaseAndGetAddressOf(), m_ShaderResourceView.ReleaseAndGetAddressOf());
	
	m_Texture2D->GetDesc(&m_Texture2DDesc);
}

void CObject2D::CTexture::SetShaderType(EShaderType eShaderType)
{
	m_eShaderType = eShaderType;
}

void CObject2D::CTexture::SetSlot(UINT Slot)
{
	m_Slot = Slot;
}

void CObject2D::CTexture::Use()
{
	switch (m_eShaderType)
	{
	case EShaderType::VertexShader:
		m_PtrDeviceContext->VSSetShaderResources(m_Slot, 1, m_ShaderResourceView.GetAddressOf());
		break;
	case EShaderType::PixelShader:
		m_PtrDeviceContext->PSSetShaderResources(m_Slot, 1, m_ShaderResourceView.GetAddressOf());
		break;
	default:
		break;
	}
}

const D3D11_TEXTURE2D_DESC& CObject2D::CTexture::GetTextureDesc() const
{
	return m_Texture2DDesc;
}
