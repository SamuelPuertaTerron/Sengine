workspace("Sengine")
	configurations { "Debug", "Release" }
	architecture("x64")

  flags
	{
		"MultiProcessorCompile"
	}

  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  
  IncludeDir = {}
  IncludeDir["BOX2D"] =     "/Source/ThirdParty/box2d/include"
  IncludeDir["ENTT"] =      "/Source/ThirdParty/entt/include"
  IncludeDir["FASTGLTF"] =  "/Source/ThirdParty/fastgltf/include"
  IncludeDir["GLAD"] =      "/Source/ThirdParty/glad/include"
  IncludeDir["GLM"] =       "/Source/ThirdParty/glm"
  IncludeDir["IMGUI"] =     "/Source/ThirdParty/imgui"
  IncludeDir["SOL2"] =      "/Source/ThirdParty/sol2/include"
  IncludeDir["STBIMAGE"] =  "/Source/ThirdParty/stb_image"
  IncludeDir["SWINDOW"] =   "/Source/ThirdParty/swindow"

  group "Dependencies"
    include "Source/ThirdParty/box2d"
    include "Source/ThirdParty/entt"
    include "Source/ThirdParty/fastgltf"
	  include "Source/ThirdParty/glad"
    include "Source/ThirdParty/glm"
    include "Source/ThirdParty/ImGui"
	  include "Source/ThirdParty/sol2"
    include "Source/ThirdParty/stb_image"
    include "Source/ThirdParty/swindow"
  group ""
  
  group "Engine"
    include "Source/Sengine"
  group ""

  group "Tools"
    include "Source/Editor"
  group ""
	
	filter "Debug"
		symbols "on"

  filter "Release"
		optimize "on"
    symbols "off"

    include "Source/Sengine"
