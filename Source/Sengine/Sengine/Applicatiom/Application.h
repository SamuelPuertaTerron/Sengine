#pragma once
#include "ISengineApp.h"

namespace Sengine
{
	class Window;
}

namespace Sengine
{
	class Application
	{
	public:

		void CreateApplication(const std::shared_ptr<ISengineApp>& app);

	private:
		[[nodiscard]] bool Init();
		void Tick();
		void Destroy();
	private:
		std::shared_ptr<ISengineApp> m_ClientApp;
		std::shared_ptr<Window> m_Window;
	};
}


