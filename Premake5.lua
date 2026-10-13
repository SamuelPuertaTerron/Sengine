include "Sengine.lua"
include "NewProject.lua"

workspace "Sengine"
	configurations { "Debug", "Release" }
	architecture "x64"
	startproject "Example Project"
	flags { "MultiProcessorCompile" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	cwd = _MAIN_SCRIPT_DIR

	SengineProjects({ Editor = true })

	-- your ProjectLoader include goes here, as it does now
	include "Example"

	for _, dir in ipairs(os.matchdirs("Games/*")) do
		if os.isfile(dir .. "/premake5.lua") then include(dir) end
	end