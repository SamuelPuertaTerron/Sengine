project "Box2d"
    kind "StaticLib"
    language "C"
    cdialect "C11"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
	{
        "include/box2d/**.h",
        "src/**.c"
    }

    includedirs 
	{
		"include",
		"src"
    }

    filter "system:windows"
        systemversion "latest"
        defines 
        {
            "F_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
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
