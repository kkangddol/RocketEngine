#include "PlaneCollider.h"

#include "DebugSystem.h"
#include "GameObject.h"
#include "Transform.h"

namespace RocketEngine
{
	PlaneCollider::PlaneCollider(GameObject* owner)
		: StaticCollider(owner)
	{

	}

	/// ����� ���� ���.
	void PlaneCollider::Update()
	{
		Matrix worldTM = gameObject->transform.GetWorldTM();
		Vector3 WHD = { 0.001f,100.0f,100.0f};
		Vector4 color = { 0.0f,0.0f,1.0f,1.0f };
		RocketCore::DebugSystem::Instance().DrawDebugBox(worldTM, WHD, true, color);
	}

	RocketEngine::Vector3 PlaneCollider::GetNormalVector() const
	{
		Vector3 axis = gameObject->transform.GetRight();
		return Vector3Normalize(axis);
	}

	float PlaneCollider::GetDistance() const
	{
		return Vector3Length(gameObject->transform.GetLocalPosition());
	}
}

