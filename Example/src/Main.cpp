#include "ExampleGlobals.h"
#include "Example/Exmaple.h"

namespace Example
{
	static int Main()
	{
		std::vector<std::unique_ptr<ILayer>> layers;
		layers.push_back(std::make_unique<Example>());

		EngineSpecification spec;
		spec.Width = 1920;
		spec.Height = 1080;
		spec.Title = "Example";
		spec.Render.VirtualWidth = 1270;
		spec.Render.VirtualHeight = 720;
		spec.Render.Scaling = ScaleMode::Integer;

		Engine::CreateAndRun(spec, std::move(layers));

		return 0;
	}
}//namespace Fallen

#ifdef FE_DEBUG
int main()
{
	return Example::Main();
}
#elif FE_RELEASE //FR_DEBUG 
	#ifdef FE_PLATFORM_WINDOWS
		#include <Windows.h>
		int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
		{
			return Example::Main();
		}
	#else //FE_PLATFORM_WINDOWS
		//Anything But Windows will use the default int main()
		int main()
		{
			return Example:Main();
		}
	#endif//FE_PLATFORM_NOT_WINDOWS 
#endif //FR_RELEASE
