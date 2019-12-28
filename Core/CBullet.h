#pragma once
#include "SharedHeader.h"

class CObject2D;

struct SBulletData
{
	DirectX::XMVECTOR Translation{};
	DirectX::XMVECTOR Velocity{};
	int ShouldDestroy{};
};

static bool BulletDataSort(const SBulletData& a, const SBulletData& b)
{
	return a.ShouldDestroy < b.ShouldDestroy;
}

class CBullet
{
public:
	CBullet(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext);
	~CBullet();

public:
	void Create(const std::string& FileName);

	void Link(CObject2D* const Object2D);

	void Spawn(float BulletSpeed);

	void Update(float DeltaTime, const DirectX::XMFLOAT2& WindowSize);

	void Draw(const DirectX::XMMATRIX& ProjectionMatrix);

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	CObject2D* m_RefObject2D{};

private:
	std::unique_ptr<CObject2D> m_Bullet{};

private:
	std::vector<SBulletData> m_vBulletData{};
};
