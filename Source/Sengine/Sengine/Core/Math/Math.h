/**
 *	Filename: TVector.h
 *	Date Created: 30/03/25
 *	Purpose: The header class for Math related functions
 ***/
#pragma once

#include "Core/Math/TVector.h"

namespace Sengine
{
	using Vector2Int = Math::TVector<int, 2>;
	using Vector2	 = Math::TVector<double, 2>;

	using Vector3Int = Math::TVector<int, 3>;
	using Vector3	 = Math::TVector<double, 3>;

	using Vector4	 = Math::TVector<double, 4>;

	using Colour	 = Math::TVector<float, 4>;
}//namespace Sengine