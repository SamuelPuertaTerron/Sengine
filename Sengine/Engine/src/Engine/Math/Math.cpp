#include "Globals.h"
#include "Math.h"

namespace Sengine::Math
{
	float DistanceSquared(Raylib::Vector2 a, Raylib::Vector2 b)
	{
		const float dx = a.x - b.x;
		const float dy = a.y - b.y;
		return dx * dx + dy * dy;
	}

	Raylib::Vector2 DirectionFromDegrees(float degrees)
	{
		const float radians = degrees * DEG2RAD;
		return Raylib::Vector2(std::cos(radians), std::sin(radians));
	}
}//namespace Sengine::Math
