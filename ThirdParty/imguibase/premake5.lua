project "ImGuiBase"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
	{
        "imgui/**.h",
        "imgui/**.cpp",
        "rlimgui/**.h",
        "rlimgui/**.cpp"
    }

    includedirs 
	{
		"imgui",
        "rlimgui",
        "../raylib/include"
    }

    filter "system:windows"
        systemversion "latest"
        defines 
        {
            "F_PLATFORM_WINDOWS",
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
