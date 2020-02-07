#pragma once
#include "SharedHeader.h"
#include <unordered_map>

class CObject2D;
class CShader;
class CPhysicsEngine;

struct SEnemyTypeData
{
	float HitPoint{};
	float Damage{};
	float Speed{};
	float BoundingCircleRadius{ 100.0f };
};

class CEnemyPool
{
public:
	CEnemyPool(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext);
	~CEnemyPool();

public:
	void Create(CShader* const VSDefault_Instanced, CShader* const PSDefault, CPhysicsEngine* PhysicsEngine);

public:
	void AddEnemyType(const std::string& EnemyTypeName, const std::string& FileName, const SEnemyTypeData& TypeData);

	void SpawnEnemy(const std::string& EnemyTypeName, const DirectX::XMVECTOR& Position, const DirectX::XMVECTOR& MovingDirection);

public:
	void Draw(const DirectX::XMMATRIX& ProjectionMatrix);

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	std::vector<std::unique_ptr<CObject2D>> m_vEnemyTypes{};
	std::vector<SEnemyTypeData> m_vEnemyTypeData{};
	std::unordered_map<std::string, size_t> m_UMapEnemyTypeNameToIndex{};

private:
	CPhysicsEngine* m_PtrPhysicsEngine{};

private:
	CShader* m_VS{};
	CShader* m_PS{};
};
