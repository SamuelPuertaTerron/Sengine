#include "Globals.h"
#include "ScriptBindingsDetail.h"

#include "Engine/Game/Components.h"

namespace Sengine::Scripting::Detail
{
	using Raylib::Vector2;
	using Raylib::Color;

	void RegisterMathTypes(sol::state& lua)
	{
		lua.new_usertype<Vector2>("Vector2",
			sol::call_constructor, sol::factories(
				[]() { return Vector2{ 0.0f, 0.0f }; },
				[](float x, float y) { return Vector2{ x, y }; }),

			"x", &Vector2::x,
			"y", &Vector2::y,

			"Length", [](const Vector2& v) { return std::sqrt(v.x * v.x + v.y * v.y); },
			"Normalized", [](const Vector2& v)
			{
				const float length = std::sqrt(v.x * v.x + v.y * v.y);
				return length > 0.0f ? Vector2{ v.x / length, v.y / length } : Vector2{ 0.0f, 0.0f };
			},

			sol::meta_function::addition, [](const Vector2& a, const Vector2& b) { return Vector2{ a.x + b.x, a.y + b.y }; },
			sol::meta_function::subtraction, [](const Vector2& a, const Vector2& b) { return Vector2{ a.x - b.x, a.y - b.y }; },
			sol::meta_function::unary_minus, [](const Vector2& v) { return Vector2{ -v.x, -v.y }; },
			sol::meta_function::multiplication, sol::overload(
				[](const Vector2& v, float s) { return Vector2{ v.x * s, v.y * s }; },
				[](float s, const Vector2& v) { return Vector2{ v.x * s, v.y * s }; }),
			sol::meta_function::to_string, [](const Vector2& v) { return std::format("({}, {})", v.x, v.y); });

		lua.new_usertype<Color>("Color",
			sol::call_constructor, sol::factories(
				[](int r, int g, int b) { return Color{ (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 }; },
				[](int r, int g, int b, int a) { return Color{ (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a }; }),

			"r", &Color::r,
			"g", &Color::g,
			"b", &Color::b,
			"a", &Color::a);

		lua.new_usertype<PhysicsMaterial>("PhysicsMaterial",
			sol::no_constructor,
			"Density", &PhysicsMaterial::Density,
			"Friction", &PhysicsMaterial::Friction,
			"Restitution", &PhysicsMaterial::Restitution);
	}
}//namespace Sengine::Scripting::Detail