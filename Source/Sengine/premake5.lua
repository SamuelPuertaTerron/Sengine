project "Sengine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    --pchheader "frpch.h"
    --pchsource "src/frpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",
        "../%{IncludeDir.GLAD}",
        "../%{IncludeDir.glm}",
		"../%{IncludeDir.stb_image}",
		"../%{IncludeDir.imgui}",
        "../%{IncludeDir.fastgltf}",
        "../%{IncludeDir.swindow}",
    }

    links
    {
        "GLAD",
        "STBIMAGE",
        "GLM",
        "IMGUI",
        "FASTGLTF",
        "SWINDOW",
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