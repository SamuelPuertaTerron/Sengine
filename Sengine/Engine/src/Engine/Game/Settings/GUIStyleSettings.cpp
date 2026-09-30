#include "Globals.h"
#include "GUIStyleSettings.h"

namespace Sengine::GUIStyleSettings
{
	namespace
	{
		namespace GUI = Raylib::GUI;

		void SetColour(int control, int property, Raylib::Color colour)
		{
			GUI::GuiSetStyle(control, property, Raylib::ColorToInt(colour));
		}
	}//namespace

	Theme GetDarkTheme()
	{
		//Built around the engine's clear colour (30, 30, 46) so UI panels sit
		//naturally on top of the game view.
		Theme theme;
		theme.Background = Raylib::Color(30, 30, 46, 255);
		theme.Surface = Raylib::Color(49, 50, 68, 255);
		theme.SurfaceHover = Raylib::Color(69, 71, 90, 255);
		theme.Accent = Raylib::Color(137, 180, 250, 255);
		theme.Border = Raylib::Color(88, 91, 112, 255);
		theme.Line = Raylib::Color(69, 71, 90, 255);
		theme.Text = Raylib::Color(205, 214, 244, 255);
		theme.TextOnAccent = Raylib::Color(30, 30, 46, 255);
		theme.DisabledSurface = Raylib::Color(24, 24, 37, 255);
		theme.DisabledBorder = Raylib::Color(49, 50, 68, 255);
		theme.DisabledText = Raylib::Color(108, 112, 134, 255);
		return theme;
	}

	void Apply(const Theme& theme)
	{
		//Start from a clean slate so switching themes never leaves stale values.
		GUI::GuiLoadStyleDefault();

		//Base properties set on DEFAULT are copied to every control by raygui.
		const int all = GUI::DEFAULT;

		SetColour(all, GUI::BORDER_COLOR_NORMAL, theme.Border);
		SetColour(all, GUI::BASE_COLOR_NORMAL, theme.Surface);
		SetColour(all, GUI::TEXT_COLOR_NORMAL, theme.Text);

		SetColour(all, GUI::BORDER_COLOR_FOCUSED, theme.Accent);
		SetColour(all, GUI::BASE_COLOR_FOCUSED, theme.SurfaceHover);
		SetColour(all, GUI::TEXT_COLOR_FOCUSED, theme.Text);

		SetColour(all, GUI::BORDER_COLOR_PRESSED, theme.Accent);
		SetColour(all, GUI::BASE_COLOR_PRESSED, theme.Accent);
		SetColour(all, GUI::TEXT_COLOR_PRESSED, theme.TextOnAccent);

		SetColour(all, GUI::BORDER_COLOR_DISABLED, theme.DisabledBorder);
		SetColour(all, GUI::BASE_COLOR_DISABLED, theme.DisabledSurface);
		SetColour(all, GUI::TEXT_COLOR_DISABLED, theme.DisabledText);

		GUI::GuiSetStyle(all, GUI::BORDER_WIDTH, theme.BorderWidth);
		GUI::GuiSetStyle(all, GUI::TEXT_PADDING, 0);

		//Extended DEFAULT properties: global, not copied per control.
		SetColour(all, GUI::BACKGROUND_COLOR, theme.Background);
		SetColour(all, GUI::LINE_COLOR, theme.Line);
		GUI::GuiSetStyle(all, GUI::TEXT_SIZE, theme.TextSize);
		GUI::GuiSetStyle(all, GUI::TEXT_SPACING, theme.TextSpacing);
		GUI::GuiSetStyle(all, GUI::TEXT_LINE_SPACING, theme.TextLineSpacing);

		//Per-control tweaks go last, because the DEFAULT calls above overwrite them.
		GUI::GuiSetStyle(GUI::BUTTON, GUI::TEXT_PADDING, theme.ButtonPadding);
		GUI::GuiSetStyle(GUI::LABEL, GUI::BORDER_WIDTH, 0);	//Labels have no frame; keeps their measured box tight.
	}

	void LoadDarkTheme()
	{
		Apply(GetDarkTheme());
	}

	void LoadDefault()
	{
		GUI::GuiLoadStyleDefault();
	}
}//namespace Sengine::GUIStyleSettings