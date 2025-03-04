#include "Globals.h"
#include "IRenderContext.h"

#include "RenderContextType.h"
#include "OpoenGL/OpenGLRenderContext.h"

namespace Sengine
{
	SharedPtr<IRenderContext> IRenderContext::Create(const ERenderContextType& contextType)
	{
		switch (contextType)
		{
		case ERenderContextType::OpenGL:
			return CreateSharedPtr<RendererGL::OpenGLRenderContext>();
		case ERenderContextType::DX11:
		case ERenderContextType::Vulkan:
		case ERenderContextType::None:
			break;
		}  

		return nullptr;
	}
}


