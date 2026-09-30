#pragma once

namespace Sengine
{
	struct PanelComponent
	{
		Raylib::Vector2 Size{ 0.0f, 0.0f };					
		std::string Title;									
		Raylib::Color BackgroundColour{ 0, 0, 0, 0 };		
		Raylib::Color BorderColour{ 0, 0, 0, 0 };			
		int16_t Layer{ 0 };									
	};

	struct TextComponent
	{
		std::string Text;
		Raylib::Color FontColour{ 0, 0, 0, 0 };				
		int FontSize = 0;									
	};

	struct ButtonComponent
	{
		std::string Text;
		Raylib::Color FontColour{ 0, 0, 0, 0 };				
		int FontSize = 0;									

		std::function<void()> ButtonClicked;
	};
}//namespace Sengine