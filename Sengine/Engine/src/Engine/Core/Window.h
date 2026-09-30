#pragma once

namespace Sengine
{
	struct EngineSpecification;

	class Window
	{
	public:
		Window(const EngineSpecification& spec);
		~Window();

		[[nodiscard]] bool GetIsWindowRunning() const;

		void PollEvents();
		void RequestClose();

		[[nodiscard]] int GetWidth() const;
		[[nodiscard]] int GetHeight() const;
	private:
		void SetIsWindowRunning(bool value);
	private:
		bool m_bIsWindowRunning;

		Raylib::Vector2 m_WindowSize;
	};
}//namespace Sengine
