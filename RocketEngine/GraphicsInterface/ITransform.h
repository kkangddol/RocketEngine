#pragma once
#include "MathHeader.h"

namespace Rocket::Core
{
	class ITransform
	{
	public:
		virtual ~ITransform() {};
		virtual void SetPosition(const Vector3& position) = 0;
		virtual void SetRotation(const Quaternion& rotation) = 0;
		virtual void SetScale(const Vector3& scale) = 0;

		virtual Vector3 GetPosition() const = 0;
		virtual Quaternion GetRotation() const = 0;
		virtual Vector3 GetScale() const = 0;
	};
}
