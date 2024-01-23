#pragma once
#include "Singleton.h"
#include "MathHeader.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

#include <vector>
#include <string>

namespace RocketCore
{
	struct DebugText
	{
		RocketEngine::Vector2 centerPos;
		std::string content;
		float size;
	};

	struct DebugBox
	{
		RocketEngine::Matrix worldTM;
		RocketEngine::Vector3 widthHeightDepth;
		bool isWire;
		RocketEngine::Vector4 color;
	};

	struct DebugLine
	{
		RocketEngine::Vector3 beginPoint;
		RocketEngine::Vector3 endPoint;
		RocketEngine::Vector4 color;
	};

	struct Debug2DBox
	{
		RocketEngine::Vector2 LT;
		RocketEngine::Vector2 RB;
		RocketEngine::Vector4 color;
	};

}

namespace RocketCore
{
	class DebugSystem : public Singleton<DebugSystem>
	{
		friend Singleton;
	private:
		DebugSystem() = default;

	public:
		void Initialize();
		void Finalize();

	public:
		void CheckDebugMode();

	public:
		void DrawDebugText(RocketEngine::Vector2 centerPos, std::string content, float size);
		void DrawDebugBox(RocketEngine::Matrix worldTM, RocketEngine::Vector3 widthHeightDepth = {1.0f,1.0f,1.0f}, bool isWire = true, RocketEngine::Vector4 color = { 1.0f,0.0f,0.0f,1.0f });
		void DrawDebugLine(RocketEngine::Vector3 beginPoint, RocketEngine::Vector3 endPoint, RocketEngine::Vector4 color = { 1.0f,0.0f,0.0f,1.0f });
		void DrawDebug2DBox(RocketEngine::Vector2 LT, RocketEngine::Vector2 RB, RocketEngine::Vector4 color = { 1.0f,0.0f,0.0f,1.0f });

	public:
		std::vector<DebugText>& GetTextQueue();
		std::vector<DebugBox>& GetBoxQueue();
		std::vector<DebugLine>& GetLineQueue();
		std::vector<Debug2DBox>& Get2DBoxQueue();

	public:
		void FlushBoxQueue();
		void FlushTextQueue();
		void FlushLineQueue();
		void Flush2DBoxQueue();

	private:
		std::vector<DebugText> _textQueue;
		std::vector<DebugBox> _boxQueue;
		std::vector<DebugLine> _lineQueue;
		std::vector<Debug2DBox> _2DBoxQueue;

	public:
		void SetDebugMode(bool isDebugMode);
		bool IsDebugMode() const;

	private:
		bool _isDebugMode;
	};
}
