#include "CBoundingCircleRep.h"
#include "CObject2D.h"

CBoundingCircleRep::CBoundingCircleRep(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
	m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
{
	assert(PtrDevice);
	assert(PtrDeviceContext);
}

CBoundingCircleRep::~CBoundingCircleRep()
{
}

void CBoundingCircleRep::Create()
{
	m_Circle = std::make_unique<CObject2D>(m_PtrDevice, m_PtrDeviceContext);

	m_Circle->Create()
}
