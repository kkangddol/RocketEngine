#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include <vector>

#if defined(_DX11) || defined(_DX12)
#include "MathHeader.h"
#endif // _DX11 || _DX12

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif //_DEBUG

namespace physx
{
	class PxScene;
}

namespace RocketEngine
{
	class GameObject;
}

namespace RocketEngine
{
	class ROCKET_API Collider : public Component
	{
	public:
		Collider(GameObject* owner);

	// Collider 자체적으로 들고 있는 TRS Offset과 그 getter, setter
	public:
		void SetPositionOffset(Vector3 position);
		void SetRotationOffset(Quaternion rotation);
		void SetRotationOffset(float angleX, float angleY, float angleZ);
		void SetScaleOffset(Vector3 scale);

		Vector3 GetPositionOffset() const;
		Quaternion GetRotationOffset() const;
		Vector3 GetScaleOffset() const;

		Matrix GetOffsetTranslateMatrix() const;
		Matrix GetOffsetRotationMatrix() const;
		Matrix GetOffsetScaleMatrix() const;

		Matrix GetOffsetTM() const;

	protected:
		Vector3 _positionOffset;
		Quaternion _rotationOffset;
		Vector3 _scaleOffset;

	public:
		virtual void UpdateToPhysics() abstract;

	protected:
		bool _wasTranslated = false;

	public:
		void SetPxScene(physx::PxScene* pxScene);
		physx::PxScene* GetPxScene();

	protected:
		physx::PxScene* _pxScene;
	};
}
