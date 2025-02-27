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
		/// <summary>
		/// The entry point of the engine. Run's the given application from the parameter. 
		/// </summary>
		void RunApplication(const std::shared_ptr<ISengineApp>& app);

	private:
		[[nodiscard]] bool Init();
		void Tick() const;
		void Destroy() const;
	private:
		std::shared_ptr<ISengineApp> m_ClientApp;
		std::shared_ptr<Window> m_Window;
	};
}


