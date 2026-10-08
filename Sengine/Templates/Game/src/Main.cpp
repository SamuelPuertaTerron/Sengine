#include "__PROJECT__Globals.h"
#include "__PROJECT__/__PROJECT__.h"

namespace __PROJECT__
{
	static int Main()
	{
		std::vector<std::unique_ptr<ILayer>> layers;
		layers.push_back(std::make_unique<GameLayer>());

		EngineSpecification spec;
		spec.Width = 1920;
		spec.Height = 1080;
		spec.Title = "__PROJECT__";
		spec.Render.VirtualWidth = 1280;
		spec.Render.VirtualHeight = 720;
		spec.Render.Scaling = ScaleMode::Integer;

		Engine::CreateAndRun(spec, std::move(layers));

		return 0;
	}
}//namespace __PROJECT__

#if defined(FE_DEBUG)
int main()
{
	return __PROJECT__::Main();
}
#elif defined(FE_RELEASE)
	#ifdef FE_PLATFORM_WINDOWS
		#include <Windows.h>
		int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
		{
			return __PROJECT__::Main();
		}
	#else //FE_PLATFORM_WINDOWS
		//Anything but Windows uses the default int main().
		int main()
		{
			return __PROJECT__::Main();
		}
	#endif //FE_PLATFORM_WINDOWS
#endif //FE_DEBUG / FE_RELEASE
