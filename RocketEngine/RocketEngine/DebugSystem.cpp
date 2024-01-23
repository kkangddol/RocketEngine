#include "DebugSystem.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include <windows.h>

namespace RocketCore
{

	void DebugSystem::Initialize()
	{

	}

	void DebugSystem::Finalize()
	{

	}

	void DebugSystem::CheckDebugMode()
	{
		if (InputSystem::Instance().GetKeyDown(VK_F1))
		{
			SetDebugMode(!_isDebugMode);
		}

		if (InputSystem::Instance().GetKeyDown(VK_F2))
		{
			SoundSystem::Instance().SoundGroupMuteAll();
		}
	}

	void DebugSystem::DrawDebugText(RocketEngine::Vector2 centerPos, std::string content, float size)
	{
		if (!_isDebugMode)
		{
			return;
		}

		_textQueue.push_back({ centerPos,content,size });
	}

	void DebugSystem::DrawDebugBox(RocketEngine::Matrix worldTM, RocketEngine::Vector3 widthHeightDepth /*= {1.0f,1.0f,1.0f}*/, bool isWire /*= true*/, RocketEngine::Vector4 color /*= { 1.0f,0.0f,0.0f,0.0f }*/)
	{
		if (!_isDebugMode)
		{
			return;
		}

		_boxQueue.push_back({ worldTM, widthHeightDepth, isWire,color });
	}

	void DebugSystem::DrawDebugLine(RocketEngine::Vector3 beginPoint, RocketEngine::Vector3 endPoint, RocketEngine::Vector4 color /*= { 1.0f,0.0f,0.0f,1.0f }*/)
	{
		if (!_isDebugMode)
		{
			return;
		}

		_lineQueue.push_back({ beginPoint, endPoint, color });
	}

	void DebugSystem::DrawDebug2DBox(RocketEngine::Vector2 LT, RocketEngine::Vector2 RB, RocketEngine::Vector4 color /*= { 1.0f,0.0f,0.0f,1.0f }*/)
	{
		if (!_isDebugMode)
		{
			return;
		}

		_2DBoxQueue.push_back({ LT, RB, color });
	}

	std::vector<RocketCore::DebugText>& DebugSystem::GetTextQueue()
	{
		return _textQueue;
	}

	std::vector<RocketCore::DebugBox>& DebugSystem::GetBoxQueue()
	{
		return _boxQueue;
	}

	std::vector<RocketCore::DebugLine>& DebugSystem::GetLineQueue()
	{
		return _lineQueue;
	}

	std::vector<RocketCore::Debug2DBox>& DebugSystem::Get2DBoxQueue()
	{
		return _2DBoxQueue;
	}

	void DebugSystem::FlushBoxQueue()
	{
		_boxQueue.clear();
	}

	void DebugSystem::FlushTextQueue()
	{
		_textQueue.clear();
	}

	void DebugSystem::FlushLineQueue()
	{
		_lineQueue.clear();
	}

	void DebugSystem::Flush2DBoxQueue()
	{
		_2DBoxQueue.clear();
	}

	void DebugSystem::SetDebugMode(bool isDebugMode)
	{
		_isDebugMode = isDebugMode;
	}

	bool DebugSystem::IsDebugMode() const
	{
		return _isDebugMode;
	}
}
