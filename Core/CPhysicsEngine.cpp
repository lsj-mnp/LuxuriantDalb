#include "CPhysicsEngine.h"
#include "CObject2D.h"

CPhysicsEngine::CPhysicsEngine()
{
}

CPhysicsEngine::~CPhysicsEngine()
{
}

void CPhysicsEngine::RegisterObject2D(CObject2D* const Object2D)
{
	if (!Object2D) return;

	m_vObject2Ds.emplace_back(Object2D);
}

void CPhysicsEngine::Execute(float DeltaTime)
{
	using namespace DirectX;

	for (auto& Object2D : m_vObject2Ds)
	{
		size_t instance_count{ Object2D->GetInstanceCount() };

		for (size_t instanceID = 0; instanceID < instance_count; ++instanceID)
		{
			Object2D->TranslateInstance(instanceID, Object2D->GetInstanceVelocity(instanceID) * DeltaTime);
		}

		Object2D->UpdateInstanceBuffer();
	}
}
