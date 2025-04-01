project "Sengine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "Globals.h"
    pchsource "Sengine/Globals.cpp"

    files
    {
        "Sengine/**.h",
        "Sengine/**.cpp",
    }

    includedirs
    {
        "Sengine",
        "%{IncludeDir.THIRDPARTY}",
    }

    links
    {
        "GLAD",
        "STBIMAGE",
        "GLM",
        "IMGUI",
        "FASTGLTF",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "SE_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines
        {
            "SE_DEBUG"
        }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
        defines
        {
            "SE_RELEASE"
        }
		runtime "Release"
        optimize "On"
        symbols "off"