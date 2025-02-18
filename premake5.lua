workspace("Sengine")
	configurations { "Debug", "Release" }
	architecture("x64")

  flags
	{
		"MultiProcessorCompile"
	}

  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  
  IncludeDir = {}
  IncludeDir["SENGINE"] =     "../Sengine/"
  IncludeDir["THIRDPARTY"] =     "../ThirdParty/"

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
    include "Source/Editor"
