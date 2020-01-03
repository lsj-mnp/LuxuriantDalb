#include "CStarPool.h"
#include "Math.h"
#include "CObject2D.h"
#include "CShader.h"

using std::make_unique;

CStarPool::CStarPool(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
	m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
{
	assert(PtrDevice);
	assert(PtrDeviceContext);
}

CStarPool::~CStarPool()
{
}

void CStarPool::Create(CShader* const VSDefault_Instanced, CShader* const PSDefault, CShader* const PSDefault_Colorize,
	size_t SStarCount, size_t SPlanetCount, size_t MPlanetCount, size_t LPlanetCount)
{
	m_VS = VSDefault_Instanced;

	m_SStar = make_unique<CObject2D>(m_PtrDevice, m_PtrDeviceContext);
	m_LPlanet = make_unique<CObject2D>(m_PtrDevice, m_PtrDeviceContext);
	m_MPlanet = make_unique<CObject2D>(m_PtrDevice, m_PtrDeviceContext);
	m_SPlanet = make_unique<CObject2D>(m_PtrDevice, m_PtrDeviceContext);

	m_SStar->CreateAsTextureSize("SStar", "Asset/small_star.png");
	m_LPlanet->CreateAsTextureSize("LPlanet", "Asset/planet_0.png");
	m_MPlanet->CreateAsTextureSize("MPlanet", "Asset/planet_1.png");
	m_SPlanet->CreateAsTextureSize("SPlanet", "Asset/planet_2.png");

	InitializeObject2D(m_SStar.get(), PSDefault_Colorize, SStarCount, -360, 1000, 5, 5);
	InitializeObject2D(m_LPlanet.get(), PSDefault, LPlanetCount, -360, 1000, 5, 15);
	InitializeObject2D(m_MPlanet.get(), PSDefault, MPlanetCount, -360, 1000, 5, 15);
	InitializeObject2D(m_SPlanet.get(), PSDefault, SPlanetCount, -360, 1000, 5, 15);
}

void CStarPool::Update(float DeltaTime)
{
	float SpeedFactor{ 1.5f };
	UpdateObject2D(m_SStar.get(), -150.0f * SpeedFactor, +370, +1370, 5, 5, DeltaTime);
	UpdateObject2D(m_LPlanet.get(), -50.0f * SpeedFactor, +370, +1370, 5, 15, DeltaTime);
	UpdateObject2D(m_MPlanet.get(), -80.0f * SpeedFactor, +370, +1370, 5, 15, DeltaTime);
	UpdateObject2D(m_SPlanet.get(), -120.0f * SpeedFactor, +370, +1370, 5, 15, DeltaTime);
}

void CStarPool::Draw(const DirectX::XMMATRIX& ProjectionMatrix)
{
	DrawObject2D(m_SStar.get(), ProjectionMatrix);
	DrawObject2D(m_LPlanet.get(), ProjectionMatrix);
	DrawObject2D(m_MPlanet.get(), ProjectionMatrix);
	DrawObject2D(m_SPlanet.get(), ProjectionMatrix);
}

void CStarPool::InitializeObject2D(CObject2D* Object2D, CShader* PS, size_t InstanceCount, int MinY, int MaxY, int MinSize, int MaxSize)
{
	Object2D->SetPixelShader(PS);

	for (size_t iInstance = 0; iInstance < InstanceCount; ++iInstance)
	{
		Object2D->InsertInstance();

		float RandomSize{ GetRandomFloat(MinSize, MaxSize) * 0.1f };
		Object2D->ScaleInstanceTo(iInstance, DirectX::XMVectorSet(RandomSize, RandomSize, 1.0f, 0.0f));

		float RandomX{ GetRandomFloat(-700, +700) };
		float RandomY{ GetRandomFloat(MinY, MaxY) };

		Object2D->TranslateInstanceTo(iInstance, DirectX::XMVectorSet(RandomX, RandomY, 0.0f, 0.0f));
	}
	Object2D->UpdateInstanceBuffer();
}

void CStarPool::UpdateObject2D(CObject2D* Object2D, float DeltaY, int MinY, int MaxY, int MinSize, int MaxSize, float DeltaTime)
{
	using namespace DirectX;

	for (size_t iInstance = 0; iInstance < Object2D->GetInstanceCount(); ++iInstance)
	{
		float Scale{ DirectX::XMVectorGetX(Object2D->GetInstanceScaling(iInstance)) };
		Object2D->RotateInstance(iInstance, DirectX::XMVectorSet(0.0f, 0.0f, 0.1f / Scale, 0.0f) * DeltaTime);
		Object2D->TranslateInstance(iInstance, DirectX::XMVectorSet(0.0f, DeltaY / Scale, 0.0f, 0.0f) * DeltaTime);

		//화면을 벗어났을 경우 위치 변경 역할을 함.
		if (DirectX::XMVectorGetY(Object2D->GetInstanceTranslation(iInstance)) < -450.0f)
		{
			float RandomSize{ GetRandomFloat(MinSize, MaxSize) * 0.1f };
			Object2D->ScaleInstanceTo(iInstance, DirectX::XMVectorSet(RandomSize, RandomSize, 1.0f, 0.0f));

			float RandomX{ GetRandomFloat(-700, +700) };
			float RandomY{ GetRandomFloat(MinY, MaxY) };
			Object2D->TranslateInstanceTo(iInstance, DirectX::XMVectorSet(RandomX, RandomY, 0.0f, 0.0f));
		}
	}
	Object2D->UpdateInstanceBuffer();
}

void CStarPool::DrawObject2D(CObject2D* Object2D, const DirectX::XMMATRIX& ProjectionMatrix)
{
	m_VS->Use();
	Object2D->GetPixelShader()->Use();

	Object2D->Draw(ProjectionMatrix);
}
