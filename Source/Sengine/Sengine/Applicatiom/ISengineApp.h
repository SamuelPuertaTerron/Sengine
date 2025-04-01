/**
 *	Filename: ISengineApp.h
 *	Date Created: 21/02/25
 *	Purpose: The client app base class.
 ***/

#pragma once

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

		/// <summary>
		/// Returns the Window Description from the client app
		/// </summary>
		[[nodiscard]] virtual WindowDescription& GetWindowDescription() = 0;
		/// <summary>
		/// Gets called before any other Initialised function is called.
		/// </summary>
		virtual bool OnEarlyInit() = 0;
		/// <summary>
		/// Gets called after everything has been Initialised. 
		/// </summary>
		virtual bool OnInit() = 0;
		/// <summary>
		/// Gets called every frame
		/// </summary>
		virtual void OnTick() = 0;
		/// <summary>
		/// Gets called before any other Destroy function
		/// </summary>
		virtual void OnDestroy() = 0;
		/// <summary>
		/// Gets called after evey Destroy function
		/// </summary>
		virtual void OnLateDestroy() = 0;
	};
}//namespace Sengine