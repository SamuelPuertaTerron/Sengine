/**
 *	Filename: Application.h
 *	Date Created: 21/02/25
 *	Purpose: The main application class, handles the functionality from int main() to when the application is closed. 
 ***/

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
		/// Creates and Instance of this object if it has not been created else returns the cached instance.
		/// </summary>
		static Application& GetInstance();

		/// <summary>
		/// The entry point of the engine. Run's the given application from the parameter. 
		/// </summary>
		void RunApplication(const std::shared_ptr<ISengineApp>& app);

		//Getter Functions

		std::shared_ptr<ISengineApp> GetClientApp() const;
		std::shared_ptr<Window> GetWindow() const;

	private:
		Application() = default;
		~Application() = default;

		[[nodiscard]] bool Init();
		void Tick() const;
		void Destroy() const;
	private:
		inline static Application* s_Instance;

		std::shared_ptr<ISengineApp> m_ClientApp;
		std::shared_ptr<Window> m_Window;
	};
}


