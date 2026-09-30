#include "Globals.h"
#include "UISystem.h"

#include <algorithm>

#include "Engine/Game/World.h"
#include "Engine/Game/Components.h"

namespace Sengine
{
	namespace
	{
		namespace GUI = Raylib::GUI;

		class ScopedStyle
		{
		public:
			ScopedStyle(int control, int property)
				: m_Control(control), m_Property(property), m_Previous(GUI::GuiGetStyle(control, property)) {
			}

			~ScopedStyle() { GUI::GuiSetStyle(m_Control, m_Property, m_Previous); }

			ScopedStyle(const ScopedStyle&) = delete;
			ScopedStyle& operator=(const ScopedStyle&) = delete;

			[[nodiscard]] int GetPrevious() const { return m_Previous; }
			void Set(int value) const { GUI::GuiSetStyle(m_Control, m_Property, value); }

			//Custom colour if it has any alpha, otherwise the theme's value.
			void SetColourOrTheme(Raylib::Color colour) const
			{
				Set(colour.a > 0 ? Raylib::ColorToInt(colour) : m_Previous);
			}

		private:
			int m_Control;
			int m_Property;
			int m_Previous;
		};

		int ScaledSpacing(int baseSpacing, int baseSize, int fontSize)
		{
			if (baseSize <= 0)
			{
				return baseSpacing;
			}
			return std::max(1, (baseSpacing * fontSize + baseSize / 2) / baseSize);
		}

		//Exact box a raygui control needs so its text is neither clipped nor re-aligned.
		Raylib::Rectangle MeasureControl(int control, const std::string& text,
			Raylib::Vector2 position, int fontSize, int spacing)
		{
			const Raylib::Vector2 size = Raylib::MeasureTextEx(GUI::GuiGetFont(), text.c_str(),
				static_cast<float>(fontSize), static_cast<float>(spacing));

			//raygui insets the bounds by border + padding on each side; add it back.
			const float inset = static_cast<float>(
				GUI::GuiGetStyle(control, GUI::BORDER_WIDTH) +
				GUI::GuiGetStyle(control, GUI::TEXT_PADDING));

			//Whole-pixel position keeps every glyph pixel the same size on the canvas.
			return Raylib::Rectangle(std::floor(position.x), std::floor(position.y),
				std::ceil(size.x) + inset * 2.0f,
				std::ceil(size.y) + inset * 2.0f);
		}
	}//namespace

	void UISystem::OnCreate(World& world) {}

	void UISystem::OnDestroy(World& world)
	{
		m_PanelOrder.clear();
	}

	void UISystem::OnTick(World& world, float deltaTime)
	{
		//Back to front: panels are the background everything else sits on.
		DrawPanels(world);
		DrawText(world);
		DrawButtons(world);
	}

	void UISystem::DrawPanels(World& world)
	{
		const ScopedStyle background(GUI::DEFAULT, GUI::BACKGROUND_COLOR);
		const ScopedStyle line(GUI::DEFAULT, GUI::LINE_COLOR);

		auto view = world.GetRegistry().view<const IdentificationComponent, const TransformComponent, const PanelComponent>();

		m_PanelOrder.clear();
		for (auto [entity, id, transform, panel] : view.each())
		{
			if (id.IsActive && panel.Size.x > 0.0f && panel.Size.y > 0.0f)
			{
				m_PanelOrder.push_back(entity);
			}
		}

		//Stable so panels on the same layer keep creation order.
		std::stable_sort(m_PanelOrder.begin(), m_PanelOrder.end(),
			[&view](entt::entity a, entt::entity b)
			{
				return view.get<const PanelComponent>(a).Layer < view.get<const PanelComponent>(b).Layer;
			});

		for (entt::entity entity : m_PanelOrder)
		{
			const TransformComponent& transform = view.get<const TransformComponent>(entity);
			const PanelComponent& panel = view.get<const PanelComponent>(entity);

			background.SetColourOrTheme(panel.BackgroundColour);
			line.SetColourOrTheme(panel.BorderColour);

			const Raylib::Rectangle bounds(
				std::floor(transform.Position.x), std::floor(transform.Position.y),
				std::ceil(panel.Size.x), std::ceil(panel.Size.y));

			GUI::GuiPanel(bounds, panel.Title.empty() ? nullptr : panel.Title.c_str());
		}
	}//Styles restored here.

	void UISystem::DrawText(World& world)
	{
		const ScopedStyle size(GUI::DEFAULT, GUI::TEXT_SIZE);
		const ScopedStyle spacing(GUI::DEFAULT, GUI::TEXT_SPACING);
		const ScopedStyle colour(GUI::LABEL, GUI::TEXT_COLOR_NORMAL);

		auto view = world.GetRegistry().view<const IdentificationComponent, const TransformComponent, const TextComponent>();
		for (auto [entity, id, transform, text] : view.each())
		{
			if (!id.IsActive || text.Text.empty())
			{
				continue;
			}

			const int fontSize = text.FontSize > 0 ? text.FontSize : size.GetPrevious();
			const int letterSpacing = ScaledSpacing(spacing.GetPrevious(), size.GetPrevious(), fontSize);

			size.Set(fontSize);
			spacing.Set(letterSpacing);
			colour.SetColourOrTheme(text.FontColour);

			const Raylib::Rectangle bounds = MeasureControl(GUI::LABEL, text.Text, transform.Position, fontSize, letterSpacing);
			GUI::GuiLabel(bounds, text.Text.c_str());
		}
	}//Styles restored here.

	void UISystem::DrawButtons(World& world)
	{
		const ScopedStyle size(GUI::DEFAULT, GUI::TEXT_SIZE);
		const ScopedStyle spacing(GUI::DEFAULT, GUI::TEXT_SPACING);
		const ScopedStyle colourNormal(GUI::BUTTON, GUI::TEXT_COLOR_NORMAL);
		const ScopedStyle colourFocused(GUI::BUTTON, GUI::TEXT_COLOR_FOCUSED);
		const ScopedStyle colourPressed(GUI::BUTTON, GUI::TEXT_COLOR_PRESSED);

		auto view = world.GetRegistry().view<const IdentificationComponent, const TransformComponent, const ButtonComponent>();
		for (auto [entity, id, transform, button] : view.each())
		{
			if (!id.IsActive || button.Text.empty())
			{
				continue;
			}

			const int fontSize = button.FontSize > 0 ? button.FontSize : size.GetPrevious();
			const int letterSpacing = ScaledSpacing(spacing.GetPrevious(), size.GetPrevious(), fontSize);

			size.Set(fontSize);
			spacing.Set(letterSpacing);
			colourNormal.SetColourOrTheme(button.FontColour);
			colourFocused.SetColourOrTheme(button.FontColour);
			colourPressed.SetColourOrTheme(button.FontColour);

			const Raylib::Rectangle bounds = MeasureControl(GUI::BUTTON, button.Text, transform.Position, fontSize, letterSpacing);

			//Copy first: the callback may remove this component or destroy the entity.
			if (GUI::GuiButton(bounds, button.Text.c_str()) && button.ButtonClicked)
			{
				auto& callback = button.ButtonClicked;
				callback();
			}
		}
	}//Styles restored here.
}//namespace Sengine