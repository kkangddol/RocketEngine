#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "Mesh.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class SphereMesh : public Mesh
	{
	public:
		SphereMesh();
		~SphereMesh();

	public:
		virtual void Initialize(ID3D11Device* device) override;

	private:
		void BuildGeometryBuffers(ID3D11Device* device, float radius = 0.5f, UINT sliceCount = 20, UINT stackCount = 20);
	};
}
