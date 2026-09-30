#pragma once

namespace Sengine
{
	class ILayer
	{
	public:
		virtual ~ILayer() = default;

		virtual void OnCreate() {}
		virtual void OnTick(float deltaTime) {}
		virtual void OnDestroy() {}
	};
}//namespace Sengine
