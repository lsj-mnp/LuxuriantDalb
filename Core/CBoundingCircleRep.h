#pragma once
#include "SharedHeader.h"

class CObject2D;

class CBoundingCircleRep
{
public:
	CBoundingCircleRep(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext);
	~CBoundingCircleRep();

public:
	void Create();

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	std::unique_ptr<CObject2D> m_Circle{};
};
