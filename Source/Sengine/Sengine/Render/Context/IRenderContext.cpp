#include "Globals.h"
#include "IRenderContext.h"

#include "RenderContextType.h"
#include "OpoenGL/OpenGLRenderContext.h"

namespace Sengine
{
	std::shared_ptr<IRenderContext> IRenderContext::Create(const ERenderContextType& contextType)
	{
		switch (contextType)
		{
		case ERenderContextType::OpenGL:
			return std::make_shared<RendererGL::OpenGLRenderContext>();
		case ERenderContextType::DX11:
		case ERenderContextType::Vulkan:
		case ERenderContextType::None:
			break;
		}  

		return nullptr;
	}
}


