/**
 *	Filename: INativeWindows.h
 *	Date Created: 27/02/25
 *	Purpose: An interface for native windows. 
 ***/
#pragma once

namespace Sengine::Windows
{
	class INativeWindow
	{
	public:
		virtual ~INativeWindow() = default;

		virtual bool Init(const WindowDescription& description) = 0;
		virtual void Destroy() = 0;

		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;

		virtual void CreateGraphicsContext() = 0;

		virtual void* GetProcAddress(const char* name) = 0;

		[[nodiscard]] static std::shared_ptr<INativeWindow> CreateNativeWindow();
	};
}//namespace Sengine::Windows


