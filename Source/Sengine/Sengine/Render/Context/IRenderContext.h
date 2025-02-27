/**
 *	Filename: IRenderContext.h
 *	Date Created: 21/02/25
 *	Purpose: Abstracts away graphic API related code. Used as a base class
 *	Last Modified: N/A
 *	Changes:	   N/A
 ***/
#pragma once

namespace Sengine
{
	enum class ERenderContextType : uint8_t;

	class IRenderContext
	{
	public:
		virtual ~IRenderContext() = default;

		[[nodiscard]] virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual void BeginRender2D() = 0;
		virtual void EndRender2D() = 0;

		static std::shared_ptr<IRenderContext> Create(const ERenderContextType& contextType);
	};
}