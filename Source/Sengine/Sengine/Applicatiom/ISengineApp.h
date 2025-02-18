#pragma once
#include <memory>

namespace Sengine
{
	struct WindowDescription;
}

namespace Sengine
{
	class ISengineApp
	{
	public:
		virtual ~ISengineApp() = default;

		[[nodiscard]] virtual WindowDescription& GetWindowDescription() = 0;
		virtual bool OnEarlyInit() = 0;
		virtual bool OnInit() = 0;
		virtual void OnTick() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnLateDestroy() = 0;
	};
}
