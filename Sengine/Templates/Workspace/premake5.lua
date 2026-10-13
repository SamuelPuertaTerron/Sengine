-- Workspace for a standalone Sengine game.
-- The engine is not inside this project: it is the shared install that SENGINE_ROOT points to.
newoption { trigger = "engine", value = "PATH", description = "Sengine install to build against (default: SENGINE_ROOT)" }

local engineRoot = _OPTIONS["engine"] or os.getenv("SENGINE_ROOT")
if not engineRoot or engineRoot == "" then
	error("SENGINE_ROOT is not set. Install the engine from the Sengine hub, or run Setup.bat in the engine folder.")
end

engineRoot = path.normalize((engineRoot:gsub("\\", "/")))
if not os.isfile(engineRoot .. "/Sengine.lua") then
	error("No Sengine install found at " .. engineRoot .. " (missing Sengine.lua). Check SENGINE_ROOT.")
end

include(engineRoot .. "/Sengine.lua")

workspace "__PROJECT__"
	configurations { "Debug", "Release" }
	architecture "x64"
	startproject "__PROJECT__"
	flags { "MultiProcessorCompile" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	cwd = _MAIN_SCRIPT_DIR

	SengineProjects()

	include "__PROJECT__"
