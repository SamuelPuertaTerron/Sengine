/**
 *	Filename: Application.h
 *	Date Created: 21/02/25
 *	Purpose: The main application class, handles the functionality from int main() to when the application is closed. 
 *	Last Modified: 26/02/25
 *	Changes:	Renamed CreateApplication to RunApplication as it make more sense
 *				Removed the while loop from the Tick function into the RunApplication function to allow for better readability
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


