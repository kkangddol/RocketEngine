#include "Component.h"
#include <typeinfo>

namespace Rocket
{
// 	Component::Component(GameObject* owner)
// 		: gameObject(owner)
// 	{
// 
// 	}

	void Component::SetRestart()
	{
		_isStarted = false;
	}

}
