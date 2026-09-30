#include "EditorGlobals.h"
#include "Editor/Editor.h"

namespace Editor
{
	static int Main()
	{
		std::vector<std::unique_ptr<ILayer>> layers;
		layers.push_back(std::make_unique<Editor>());

		EngineSpecification spec;
		spec.Width = 1920;
		spec.Height = 1080;
		spec.Title = "Editor";
		spec.Render.VirtualWidth = 1920;
		spec.Render.VirtualHeight = 1080;
		spec.Render.Scaling = ScaleMode::Integer;

		Engine::CreateAndRun(spec, std::move(layers));

		return 0;
	}
}//namespace Editor

#ifdef FE_DEBUG
int main()
{
	return Editor::Main();
}
#elif FE_RELEASE //FR_DEBUG 

	#ifdef FE_PLATFORM_WINDOWS
		#include <Windows.h>
		int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
		{
			return Editor::Main();
		}
	#else //FE_PLATFORM_WINDOWS
		//Anything But Windows will use the default int main()
		int main()
		{
			return Editor::Main();
		}
	#endif // Other Platforms
#endif //FR_RELEASE
