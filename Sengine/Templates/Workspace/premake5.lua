-- Workspace for a standalone Sengine game. The engine lives in the Sengine/ submodule.
if not os.isfile(_MAIN_SCRIPT_DIR .. "/Sengine/Sengine.lua") then
	error("Sengine/Sengine.lua not found. Run: git submodule update --init --recursive")
end

include "Sengine/Sengine.lua"

workspace "__PROJECT__"
	configurations { "Debug", "Release" }
	architecture "x64"
	startproject "__PROJECT__"
	flags { "MultiProcessorCompile" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	cwd = _MAIN_SCRIPT_DIR

	SengineProjects()

	include "__PROJECT__"
