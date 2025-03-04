project "NLOHMANN"
kind "StaticLib"
language "C++"
cppdialect "C++17"
staticruntime "off"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

files
{
	"**.h",
	"**.hpp",
	"**.cpp"
}

filter "system:linux"
    pic "On"

    systemversion "latest"
    staticruntime "On"

filter "system:windows"
    systemversion "latest"
    staticruntime "On"

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
    }

filter "configurations:Debug"
    runtime "Debug"
    symbols "On"

filter "configurations:Release"
    runtime "Release"
    optimize "On"
    symbols "off"