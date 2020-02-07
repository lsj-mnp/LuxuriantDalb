#include "CBoundingCircleRep.h"
#include "CObject2DLine.h"

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
	std::vector<S2DLine> v2DLines{};

	int count{ 32 };

	DirectX::XMVECTOR color{ DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f) };

	float radius{ 1.0f };

	float theta_unit{ DirectX::XM_2PI / (float)count };

	for (int i = 0; i < count; ++i)
	{
		float theta0{ theta_unit * i };
		float theta1{ theta_unit * (i + 1) };

		S2DLine Line{};
		Line.VertexA = SVertex2DLine(DirectX::XMVectorSet(radius * cosf(theta0), radius * sinf(theta0), 0.0f, 1.0f), color);
		Line.VertexB = SVertex2DLine(DirectX::XMVectorSet(radius * cosf(theta1), radius * sinf(theta1), 0.0f, 1.0f), color);

		v2DLines.emplace_back(Line);
	}
	m_Circle = std::make_unique<CObject2DLine>(m_PtrDevice, m_PtrDeviceContext);
	m_Circle->Create(v2DLines);
}

void CBoundingCircleRep::Draw(const DirectX::XMMATRIX& ProjectionMatrix)
{
	m_Circle->Draw(ProjectionMatrix);
}

CObject2DLine* CBoundingCircleRep::GetObject2DLine()
{
	return m_Circle.get();
}
