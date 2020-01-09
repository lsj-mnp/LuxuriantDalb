#pragma once
#include "SharedHeader.h"

class CObject2D;

class CPhysicsEngine
{
public:
	CPhysicsEngine();
	~CPhysicsEngine();

public:
	void RegisterObject2D(CObject2D* const Object2D);

	void Execute(float DeltaTime);

private:
	std::vector<CObject2D*> m_vObject2Ds{};
};