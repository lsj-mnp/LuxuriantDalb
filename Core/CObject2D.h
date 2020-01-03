#pragma once

#include "SharedHeader.h"
#include <unordered_map>

class CShader;
class CConstantBuffer;

struct SVertex2D
{
	SVertex2D() {}
	SVertex2D(DirectX::XMFLOAT2 _Position, DirectX::XMFLOAT2 _TexCoord) : Position{ _Position }, TexCoord{ _TexCoord }{}

	DirectX::XMFLOAT2 Position{};
	DirectX::XMFLOAT2 TexCoord{};
};

struct STriangle
{
	STriangle() {}
	STriangle(UINT _0, UINT _1, UINT _2) : Index{ _0, _1, _2 } {}

	UINT Index[3]{};
};

struct SInstanceCPUData
{
	std::string Name{};
	DirectX::XMVECTOR Translation{ 0, 0, 0, 1 };
	DirectX::XMVECTOR Rotation{};
	DirectX::XMVECTOR Scaling{ 1, 1, 1, 0 };
};

struct SInstanceGPUData
{
	DirectX::XMMATRIX WorldMatrix{ DirectX::XMMatrixIdentity() };
};

class CObject2D
{
public:
	struct SComponentTransform
	{
		DirectX::XMMATRIX WorldMatrix{ DirectX::XMMatrixIdentity() };
		DirectX::XMVECTOR Translation{ 0, 0, 0, 1 };
		DirectX::XMVECTOR Rotation{};
		DirectX::XMVECTOR Scaling{ 1, 1, 1, 0 };
	};

	struct SCBSpace
	{
		DirectX::XMMATRIX WorldMatrix{};
		DirectX::XMMATRIX ProjectionMatrix{};
		DirectX::XMMATRIX WorldProjection{};
	};

public:
	class CModel2D
	{
	public:
		CModel2D(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
			m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
		{
			assert(PtrDevice);
			assert(PtrDeviceContext);
		}
		~CModel2D() {}

	public:
		void CreateRectangle(const DirectX::XMFLOAT2& Size);

		void UpdateRectangleTexCoord(const DirectX::XMFLOAT2& UVOffset, const DirectX::XMFLOAT2& UVSize);

		void SetInputBuffers();

	public:
		const DirectX::XMFLOAT2& GetSize() const;

		UINT GetIndexCount() const;

	private:
		ID3D11Device* const m_PtrDevice{};
		ID3D11DeviceContext* const m_PtrDeviceContext{};

	private:
		std::vector<SVertex2D> m_vVertices{};
		std::vector<STriangle> m_vTriangles{};
		DirectX::XMFLOAT2 m_Size{};

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer{};
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer{};
		UINT m_VBStride{ sizeof(SVertex2D) };
		UINT m_VBOffset{};
	};

public:
	class CTexture
	{
	public:
		CTexture(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
			m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
		{
			assert(PtrDevice);
			assert(PtrDeviceContext);
		}
		~CTexture() {}

	public:
		void CreateFromFile(const std::string& FileName);

		void SetShaderType(EShaderType eShaderType);

		void SetSlot(UINT Slot);

		void Use();

		const D3D11_TEXTURE2D_DESC& GetTextureDesc() const;

	private:
		ID3D11Device* const m_PtrDevice{};
		ID3D11DeviceContext* const m_PtrDeviceContext{};

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture2D{};
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView{};

	private:
		EShaderType m_eShaderType{};
		UINT m_Slot{};
		D3D11_TEXTURE2D_DESC m_Texture2DDesc{};
	};

public:
	CObject2D(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext);
	~CObject2D();

public:
	void* operator new(size_t ByteSize)
	{
		return _aligned_malloc(ByteSize, 16);
	}

	void operator delete(void* Ptr)
	{
		_aligned_free(Ptr);
	}

public:
	void Create(const std::string& Name, const DirectX::XMFLOAT2& Size, const std::string& TextureFileName);

	void CreateAsTextureSize(const std::string& Name, const std::string& TextureFileName);

	void SetVisibleArea(const DirectX::XMFLOAT2& PixelSpaceOffset);

	void SetPixelShader(CShader* const PixelShader);

	CShader* GetPixelShader();

public:
	void Draw(const DirectX::XMMATRIX& ProjectionMatrix);

public:
	void Translate(const DirectX::XMVECTOR& Translation);

	void Rotate(const DirectX::XMVECTOR& Rotation);

	void Scale(const DirectX::XMVECTOR& Scaling);

	void TranslateTo(const DirectX::XMVECTOR& Translation);

	void RotateTo(const DirectX::XMVECTOR& Rotation);

	void ScaleTo(const DirectX::XMVECTOR& Scaling);

	const DirectX::XMMATRIX& GetWorldMatrix() const;

	const DirectX::XMVECTOR& GetTranslation() const;

	const DirectX::XMVECTOR& GetRotation() const;

private:
	void UpdateWorldMatrix();

public:
	bool InsertInstance(const std::string& Name);

	bool InsertInstance();

	void TranslateInstance(size_t InstanceID, const DirectX::XMVECTOR& Translation);
	void TranslateInstance(const std::string& Name, const DirectX::XMVECTOR& Translation);
	void TranslateInstanceTo(size_t InstanceID, const DirectX::XMVECTOR& Translation);
	void TranslateInstanceTo(const std::string& Name, const DirectX::XMVECTOR& Translation);

	void RotateInstance(size_t InstanceID, const DirectX::XMVECTOR& Rotation);
	void RotateInstance(const std::string& Name, const DirectX::XMVECTOR& Rotation);
	void RotateInstanceTo(size_t InstanceID, const DirectX::XMVECTOR& Rotation);
	void RotateInstanceTo(const std::string& Name, const DirectX::XMVECTOR& Rotation);

	void ScaleInstance(size_t InstanceID, const DirectX::XMVECTOR& Scaling);
	void ScaleInstance(const std::string& Name, const DirectX::XMVECTOR& Scaling);
	void ScaleInstanceTo(size_t InstanceID, const DirectX::XMVECTOR& Scaling);
	void ScaleInstanceTo(const std::string& Name, const DirectX::XMVECTOR& Scaling);

	bool HasInstances() const;

	size_t GetInstanceCount() const;

	const std::string& GetLastInstanceName() const;

	const DirectX::XMVECTOR& GetInstanceTranslation(size_t InstanceID) const;

	const DirectX::XMVECTOR& GetInstanceTranslation(const std::string& Name) const;

	const DirectX::XMVECTOR& GetInstanceScaling(size_t InstanceID) const;

	void UpdateInstanceBuffer();

private:
	void CreateInstanceBuffer();

	void UpdateInstanceWorldMatrix(size_t InstanceID);

	void UpdateInstanceWorldMatrix(const std::string& Name);

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	std::string m_Name{};
	std::unique_ptr<CModel2D> m_Model2D{};
	std::unique_ptr<CTexture> m_Texture{};
	SComponentTransform m_ComponentTransform{};

private:
	std::vector<SInstanceCPUData> m_vInstanceCPUData{};
	std::vector<SInstanceGPUData> m_vInstanceGPUData{};
	std::unordered_map<std::string, size_t> m_UMapInstanceNameToIndex{};

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceBuffer{};
	UINT m_IBStride{ sizeof(SInstanceGPUData) };
	UINT m_IBOffset{};

private:
	CShader* m_PtrPixelShader{};

private:
	std::unique_ptr<CConstantBuffer> m_CBSpace{};
	SCBSpace m_CBSpaceData{};
};
