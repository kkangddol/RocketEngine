#include "MeshRendererBase.h"
#include "EngineProcess.h"
#include "GameObject.h"
#include "Transform.h"

namespace Rocket::Core
{
	void MeshRendererBase::SetRenderData()
	{
		_renderable->SetWorldTM(gameObject->transform.GetWorldTM());
		_renderable->SetActive(gameObject->IsActive());
	}

}
