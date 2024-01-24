#pragma once
#include "DLLExporter.h"
#include "DynamicCollider.h"
#include "MathHeader.h"

namespace RocketEngine
{
	class GameObject;

	class ROCKET_API SphereCollider final : public DynamicCollider
	{
	public:
		SphereCollider(GameObject* owner);

		/// 디버그 정보 보내기.
	protected:
		virtual void Update() override;

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	public:
		float GetRadius() const;
		void SetRadius(float radius);

	private:
		float _radius;
	};
}
