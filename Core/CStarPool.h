#pragma once

#include "SharedHeader.h"

class CObject2D;
class CShader;

class CStarPool
{
public:
	CStarPool(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext);
	~CStarPool();

public:
	void Create(CShader* const VSDefault_Instanced, CShader* const PSDefault, CShader* const PSDefault_Colorize,
		size_t SStarCount, size_t SPlanetCount, size_t MPlanetCount, size_t LPlanetCount);

	void Update();

	void Draw(const DirectX::XMMATRIX& ProjectionMatrix);

private:
	void InitializeObject2D(CObject2D* Object2D, CShader* Shader, size_t InstanceCount, int MinY, int MaxY, int MinSize, int MaxSize);

	void UpdateObject2D(CObject2D* Object2D, float DeltaY, int MinY, int MaxY, int MinSize, int MaxSize);

	void DrawObject2D(CObject2D* Object2D, const DirectX::XMMATRIX& ProjectionMatrix);

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	std::unique_ptr<CObject2D> m_SStar{};
	std::unique_ptr<CObject2D> m_LPlanet{};
	std::unique_ptr<CObject2D> m_MPlanet{};
	std::unique_ptr<CObject2D> m_SPlanet{};

private:
	CShader* m_VS{};
};
