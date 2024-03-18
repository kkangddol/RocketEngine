#pragma once
/// <summary>
/// 렌더러도 게임엔진에게 요청할 동작들이 있기 때문에
/// 게임엔진의 GraphicsSystem를 인터페이스로 받아서 사용한다..
/// GraphicsSystem에서는 그래픽스 엔진이 오브젝트를 생성하면 GameObject를 생성한다거나 하는 그런.. 동작들을 수행할 예정이다..
/// 
/// 그것을 Common에 노드를 두고..? 그걸로 GameObject를 구성하는 식으로..? 음..?
/// </summary>

namespace Rocket::Core
{
	class IGraphicsSystem
	{

	};
}
