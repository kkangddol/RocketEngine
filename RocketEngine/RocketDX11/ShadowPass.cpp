#include "ShadowPass.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"

namespace Rocket::Core
{

	ShadowPass::ShadowPass()
	{

	}

	ShadowPass::~ShadowPass()
	{

	}

	void ShadowPass::Initialize(VertexShader* vs, PixelShader* ps)
	{
		_vertexShader = vs;
		_pixelShader = ps;
	}

	void ShadowPass::GenerateShadowMap(ID3D11DeviceContext* deviceContext)
	{

	}

}
