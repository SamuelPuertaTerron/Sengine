project "Sengine Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

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
		"../%{IncludeDir.sol2}",
	}

	links
	{
		"Sengine"	
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SE_PLATFORM_WINDOWS",
		}

		postbuildcommands 
		{
			"{COPYDIR} %[Resources] %[%{cfg.targetdir}/Resources]"
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
		kind "WindowedApp"
		runtime "Release"
        optimize "on"