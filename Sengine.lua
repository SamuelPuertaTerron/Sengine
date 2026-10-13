-- Sengine.lua (engine repo root)
-- Everything a workspace needs to build against Sengine. Used by the engine's own workspace and by
-- game workspaces, which find this file through the SENGINE_ROOT environment variable.
local engineRoot = path.getdirectory(_SCRIPT)
SengineRoot = engineRoot

IncludeDir = {}
IncludeDir["Engine"]       = engineRoot .. "/Sengine/Engine/src"
IncludeDir["RaylibDir"]    = engineRoot .. "/ThirdParty/raylib/include"
IncludeDir["Entt"]         = engineRoot .. "/ThirdParty/entt"
IncludeDir["Box2D"]        = engineRoot .. "/ThirdParty/box2d/include"
IncludeDir["ImGuiBase"]    = engineRoot .. "/ThirdParty/imguibase"
IncludeDir["Nlohmannjson"] = engineRoot .. "/ThirdParty/nlohmannjson/include"
IncludeDir["Sol2"]         = engineRoot .. "/ThirdParty/sol2/include"

-- Call inside a workspace block, after setting outputdir and cwd.
function SengineProjects(options)
	options = options or {}

	-- The engine and third-party libraries build into the engine install's own bin/ folder,
	-- so every game shares one build instead of compiling the whole engine again per project.
	-- Their premake files read cwd when included, so point it at the engine for the duration.
	local projectCwd = cwd
	cwd = engineRoot

	group "Sengine"
		include(engineRoot .. "/Sengine/Engine")
		include(engineRoot .. "/Sengine/ProjectLoader")
		if options.Editor then include(engineRoot .. "/Sengine/Editor") end

	group "ThirdParty"
		include(engineRoot .. "/ThirdParty/Box2d")
		include(engineRoot .. "/ThirdParty/Entt")
		include(engineRoot .. "/ThirdParty/ImGuiBase")
		include(engineRoot .. "/ThirdParty/raylib")
		include(engineRoot .. "/ThirdParty/nlohmannjson")
		include(engineRoot .. "/ThirdParty/sol2")
	group ""

	cwd = projectCwd
end
