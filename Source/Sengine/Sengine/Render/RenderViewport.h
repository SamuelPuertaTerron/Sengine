/**
 *	Filename: Render.h
 *	Date Created: 01/04/25
 *	Purpose: A data struct to act as the viewport for rendering
 ***/
#pragma once

namespace Sengine::Render
{
	class RenderViewport
	{
	public:
		RenderViewport() = default;
		~RenderViewport() = default;
	public:
		Vector2Int Size   = {0, 0};
		Vector2Int Depth  = {0, 1};
		Vector2Int Offset = {0, 0};

	};
}//namespace Sengine::Render