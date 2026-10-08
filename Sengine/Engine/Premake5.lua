project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir (cwd.. "/bin/" .. outputdir .. "/%{prj.name}")
    objdir (cwd.. "/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "Globals.h"
    pchsource "src/Globals.cpp"

    files 
	{
        "src/**.h",
        "src/**.cpp"
    }

    includedirs 
	{
        "%{IncludeDir.RaylibDir}",
        "%{IncludeDir.Engine}",
        "%{IncludeDir.Entt}",
        "%{IncludeDir.Box2D}",
        "%{IncludeDir.ImGuiBase}",
        "%{IncludeDir.Nlohmannjson}",
        "%{IncludeDir.Sol2}"
    }

    links 
	{
        "raylib",
        "Box2d",
        "Entt",
        "ImGuiBase",
        "nlohmann",
        "sol2"
    }

    filter "files:src/Engine/Platform/**.cpp"
        flags { "NoPCH" }
    filter {}

    filter "system:windows"
        systemversion "latest"
        defines 
        {
            "FE_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "system:linux"
        pic "On"
        defines { "FE_PLATFORM_LINUX" }

        links {
            "GL",
            "pthread",
            "dl",
            "X11",
            "m"
        }

        buildoptions {
            "-Wall",
            "-Wextra",
            "-Wno-unknown-pragmas"
        }

    filter "configurations:Debug"
        defines { "FE_DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "FE_RELEASE" }
        runtime "Release"
        symbols "off"
        optimize "on"
