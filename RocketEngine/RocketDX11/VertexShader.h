#pragma once
#include <wrl.h>
#include "ShaderBase.h"
#include "VertexStruct.h"

namespace Rocket::Core
{
	class VertexShader : public ShaderBase
	{
	public:
		VertexShader();
		~VertexShader();

	public:
		virtual void Initialize(ID3D11Device* device, const std::wstring& path) override;

	public:
		ID3D11VertexShader* GetVertexShader() const;
		ID3D11Buffer* GetMatrixBuffer() const;
		ID3D11Buffer** GetAddressOfMatrixBuffer();
		ID3D11InputLayout* GetInputLayout() const;
		ID3D11SamplerState** GetAddressOfSampleState();

	public:
		void SetVertexType(eVertexType type);
		eVertexType GetVertexType() const;

	private:
		void ReflectShader(ID3D11Device* device, const std::wstring& path);
		void CreateShaderAndInputLayout(ID3D11Device* device, const std::wstring& path);
		void CreateMatrixBuffer(ID3D11Device* device);
		void CreateSamplerState(ID3D11Device* device);

	private:
		ComPtr<ID3D11VertexShader> _vertexShader;
		ComPtr<ID3D11Buffer> _matrixBuffer;
		ComPtr<ID3D11InputLayout> _inputLayout;
		ComPtr<ID3D11SamplerState> _sampleState;
		eVertexType _vertexType;
		unsigned int _numElements;
	};
}


template <typename T>
ULONG GetRefCount(const ComPtr<T>& p)
{
	T* temp = p.Get();

	ULONG ret = 0;
	if (temp != nullptr)
	{
		ret = temp->AddRef();
		ret = temp->Release();
	}

	return ret;
}
