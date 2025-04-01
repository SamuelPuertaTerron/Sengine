/**
 *	Filename: Application.h
 *	Date Created: 21/02/25
 *	Purpose: The main application class, handles the functionality from int main() to when the application is closed. 
 ***/

#pragma once
#include "ISengineApp.h"

namespace Sengine
{
	struct ApplicationState;
}

namespace Sengine
{
	class Window;

	class Application
	{
	public:
		/**
		* @brief Creates or Gets an instance of this class.
		*/
		static Application& GetInstance();

		/**
		 * @brief Initializes and starts the application's main loop.
		 * @param app Shared pointer to the engine interface.
		 * @details This method blocks until the application exits. The provided
		 *          ISengineApp implementation defines the application-specific behavior.
		 */
		void RunApplication(const std::shared_ptr<ISengineApp>& app);

		/**
		 * @brief Quits the application
		 */
		void Quit() const;

		//Getter Functions

		[[nodiscard]] ApplicationState* GetState() const;
		[[nodiscard]] WindowDescription GetWindowDescription() const;

		[[nodiscard]] std::shared_ptr<ISengineApp> GetClientApp() const;
		[[nodiscard]] std::shared_ptr<Window> GetWindow() const;

	private:
		Application() = default;
		~Application() = default;

		[[nodiscard]] bool Init();
		void Tick() const;
		void Destroy() const;

		[[nodiscard]] bool EarlyInit();
		[[nodiscard]] bool ApplicationInit();
		[[nodiscard]] bool LateInit();
	private:
		inline static Application* s_pInstance;
		ApplicationState* m_pAppState;

		std::shared_ptr<ISengineApp> m_ClientApp;
		std::shared_ptr<Window> m_Window;
	};
}//namespace Sengine