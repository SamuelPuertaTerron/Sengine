#pragma once

namespace Sengine::GUIStyleSettings
{
	struct Theme
	{
		//Colours
		Raylib::Color Background{};			//Window, panel and group backgrounds.
		Raylib::Color Surface{};			//Control fill at rest (buttons, text boxes...).
		Raylib::Color SurfaceHover{};		//Control fill under the mouse.
		Raylib::Color Accent{};				//Pressed fill, focus borders, slider/progress fill.
		Raylib::Color Border{};				//Control outline at rest.
		Raylib::Color Line{};				//Separators and grid lines.
		Raylib::Color Text{};				//Normal and hovered text.
		Raylib::Color TextOnAccent{};		//Text drawn on the Accent fill (pressed state).
		Raylib::Color DisabledSurface{};
		Raylib::Color DisabledBorder{};
		Raylib::Color DisabledText{};

		//Metrics, in virtual pixels.
		int TextSize = 20;					//Keep to multiples of the font's base size (10 for the default font).
		int TextSpacing = 2;				//Letter gap at TextSize. UISystem scales it for other sizes.
		int TextLineSpacing = 30;			//Distance between lines of multi-line text.
		int BorderWidth = 1;
		int ButtonPadding = 4;				//Space between a button's border and its text.
	};

	[[nodiscard]] Theme GetDarkTheme();

	void Apply(const Theme& theme);

	void LoadDarkTheme();					
	void LoadDefault();						//raygui's stock light theme.
}//namespace Sengine::GUIStyleSettings