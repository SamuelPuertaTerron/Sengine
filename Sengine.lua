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

	group "Sengine"
		include(engineRoot .. "/Sengine/Engine")
		if options.Editor then include(engineRoot .. "/Sengine/Editor") end

	group "ThirdParty"
		include(engineRoot .. "/ThirdParty/Box2d")
		include(engineRoot .. "/ThirdParty/Entt")
		include(engineRoot .. "/ThirdParty/ImGuiBase")
		include(engineRoot .. "/ThirdParty/raylib")
		include(engineRoot .. "/ThirdParty/nlohmannjson")
		include(engineRoot .. "/ThirdParty/sol2")
	group ""
end