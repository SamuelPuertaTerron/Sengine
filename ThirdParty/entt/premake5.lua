project "Entt"
	language "C++"
	cppdialect "C++17"
    kind "StaticLib"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "entt/**.hpp",
        "src/entt.cpp"
	}

	includedirs
	{		
        "entt",
	}

	filter "configurations:Debug"
        defines
        {
            "FE_DEBUG"
        }
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
        defines
        {
            "FE_RELEASE"
        }
		runtime "Release"
        optimize "on"
		symbols	 "off"