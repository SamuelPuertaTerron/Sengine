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
		void RunApplication(const SharedPtr<ISengineApp>& app);

		//Getter Functions

		[[nodiscard]] SharedPtr<ISengineApp> GetClientApp() const;
		[[nodiscard]] SharedPtr<Window> GetWindow() const;

	private:
		Application() = default;
		~Application() = default;

		[[nodiscard]] bool Init();
		void Tick() const;
		void Destroy() const;
	private:
		inline static Application* s_Instance;

		SharedPtr<ISengineApp> m_ClientApp;
		SharedPtr<Window> m_Window;
	};
}


