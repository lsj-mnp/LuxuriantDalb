#include "CEnemyPool.h"
#include "CObject2D.h"
#include "CShader.h"
#include "CPhysicsEngine.h"

CEnemyPool::CEnemyPool(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
	m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
{
	assert(PtrDevice);
	assert(PtrDeviceContext);
}

CEnemyPool::~CEnemyPool()
{
}

void CEnemyPool::Create(CShader* const VSDefault_Instanced, CShader* const PSDefault, CPhysicsEngine* PhysicsEngine)
{
	m_VS = VSDefault_Instanced;
	m_PS = PSDefault;

	m_PtrPhysicsEngine = PhysicsEngine;
}

void CEnemyPool::AddEnemyType(const std::string& EnemyTypeName, const std::string& FileName, const SEnemyTypeData& TypeData)
{
	m_vEnemyTypes.emplace_back(std::make_unique<CObject2D>(m_PtrDevice, m_PtrDeviceContext));

	m_vEnemyTypes.back()->CreateAsTextureSize(EnemyTypeName, FileName);

	m_vEnemyTypeData.emplace_back(TypeData);

	m_UMapEnemyTypeNameToIndex[EnemyTypeName] = m_vEnemyTypes.size() - 1;

	m_PtrPhysicsEngine->RegisterObject2D(m_vEnemyTypes.back().get());
}

void CEnemyPool::SpawnEnemy(const std::string& EnemyTypeName, const DirectX::XMVECTOR& Position, const DirectX::XMVECTOR& MovingDirection)
{
	using namespace DirectX;

	size_t enemy_type_index{ m_UMapEnemyTypeNameToIndex.at(EnemyTypeName) };

	auto& enemy_type{ m_vEnemyTypes[enemy_type_index] };

	auto& enemy_type_data{ m_vEnemyTypeData[enemy_type_index] };

	enemy_type->InsertInstance();

	enemy_type->TranslateInstanceTo(enemy_type->GetInstanceCount() - 1, Position);

	enemy_type->SetInstanceVelocity(enemy_type->GetInstanceCount() - 1, MovingDirection * enemy_type_data.Speed);
}

void CEnemyPool::Draw(const DirectX::XMMATRIX& ProjectionMatrix)
{
	for (auto& Enemy : m_vEnemyTypes)
	{
		Enemy->Draw(ProjectionMatrix);
	}
}
