project "__PROJECT__"
	language "C++"
	cppdialect "C++20"
	kind "ConsoleApp"

	targetdir (cwd .. "/bin/" .. outputdir .. "/%{prj.name}")
	objdir (cwd .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "__PROJECT__Globals.h"
	pchsource "src/__PROJECT__Globals.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
		"%{IncludeDir.Engine}",
		"%{IncludeDir.RaylibDir}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.Entt}",
		"%{IncludeDir.ImGuiBase}",
		"%{IncludeDir.Nlohmannjson}",
		"%{IncludeDir.Sol2}"
	}

	links
	{
		"Engine",
		"raylib",
	}

	postbuildcommands
	{
		"{COPYDIR} Resources \"%{cfg.targetdir}/Resources\""
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FE_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}

		links
		{
			"opengl32",
			"gdi32",
		}

	filter "system:linux"
		defines { "FE_PLATFORM_LINUX" }

		links
		{
			"GL",
			"pthread",
			"dl",
			"X11",
			"m"
		}

	filter "configurations:Debug"
		defines { "FE_DEBUG" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines { "FE_RELEASE" }
		kind "WindowedApp"
		runtime "Release"
		optimize "on"
		symbols "off"

	filter {}
