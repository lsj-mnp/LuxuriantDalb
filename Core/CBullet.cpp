#include "CBullet.h"
#include "CObject2D.h"
#include <algorithm>

using std::make_unique;

CBullet::CBullet(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
	m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
{
	assert(PtrDevice);
	assert(PtrDeviceContext);
}

CBullet::~CBullet()
{
}

void CBullet::Create(const std::string& FileName)
{
	m_Bullet = make_unique<CObject2D>(m_PtrDevice, m_PtrDeviceContext);

	m_Bullet->CreateAsTextureSize("Bullet", FileName);
}

void CBullet::Link(CObject2D* const Object2D)
{
	assert(Object2D);

	m_RefObject2D = Object2D;
}

void CBullet::Spawn(float BulletSpeed)
{
	using namespace DirectX;

	assert(m_RefObject2D);

	float angle{ DirectX::XMVectorGetZ(m_RefObject2D->GetRotation()) };

	DirectX::XMMATRIX rotation_matrix{ DirectX::XMMatrixRotationZ(angle) };

	DirectX::XMVECTOR up{ 0, 1.0f, 0, 0 };

	DirectX::XMVECTOR direction{ DirectX::XMVector3TransformNormal(up, rotation_matrix) };

	m_vBulletData.emplace_back();

	m_vBulletData.back().Translation = m_RefObject2D->GetTranslation();

	m_vBulletData.back().Velocity = direction * BulletSpeed;
}

void CBullet::Update(float DeltaTime, const DirectX::XMFLOAT2& WindowSize)
{
	using namespace DirectX;

	for (auto& BulletData : m_vBulletData)
	{
		BulletData.Translation += BulletData.Velocity * DeltaTime;

		float TranslationX{ DirectX::XMVectorGetX(BulletData.Translation) };
		float TranslationY{ DirectX::XMVectorGetY(BulletData.Translation) };

		if (TranslationX < -WindowSize.x * 0.51f || TranslationX > +WindowSize.x * 0.51f ||
			TranslationY < -WindowSize.y * 0.51f || TranslationY > +WindowSize.y * 0.51f)
		{
			BulletData.ShouldDestroy = 1;
		}
	}

	size_t DestroyCount{};
	for (auto& BulletData : m_vBulletData)
	{
		if (BulletData.ShouldDestroy != 0) ++DestroyCount;
	}
	std::sort(m_vBulletData.begin(), m_vBulletData.end(), BulletDataSort);
	for (size_t i = 0; i < DestroyCount; ++i)
	{
		m_vBulletData.pop_back();
	}
}

void CBullet::Draw(const DirectX::XMMATRIX& ProjectionMatrix)
{
	for (const auto& BulletData : m_vBulletData)
	{
		m_Bullet->TranslateTo(BulletData.Translation);

		m_Bullet->Draw(ProjectionMatrix);
	}
}
