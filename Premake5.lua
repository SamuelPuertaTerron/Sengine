include "NewProject.lua"

workspace("Sengine")
    configurations {"Debug", "Release"}
    architecture("x64")
    startproject "Example Project"

    flags {"MultiProcessorCompile"}

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    cwd = _MAIN_SCRIPT_DIR

    local sengine_dir = _MAIN_SCRIPT_DIR .. "/Sengine/"
    local games_dir = _MAIN_SCRIPT_DIR .. "/Games/"

    IncludeDir = {}
    IncludeDir["Engine"] = sengine_dir .. "Engine/src"
    IncludeDir["RaylibDir"] = cwd .. "/ThirdParty/raylib/include"
    IncludeDir["Entt"] = cwd .. "/ThirdParty/entt"
    IncludeDir["Box2D"] = cwd .. "/ThirdParty/box2d/include"
    IncludeDir["ImGuiBase"] = cwd .. "/ThirdParty/imguibase"
    IncludeDir["Nlohmannjson"] = cwd .. "/ThirdParty/nlohmannjson/include"
    IncludeDir["Sol2"] = cwd .. "/ThirdParty/sol2/include"

    group "Sengine"
        include(sengine_dir .. "Editor")
        include(sengine_dir .. "Engine")
        include(sengine_dir .. "ProjectLoader")
    group ""

    include "Example"

    group "ThirdParty"
        include "ThirdParty/Box2d"
        include "ThirdParty/Entt"
        include "ThirdParty/ImGuiBase"
        include "ThirdParty/raylib"
        include "ThirdParty/nlohmannjson"
        include "ThirdParty/sol2"
    group ""