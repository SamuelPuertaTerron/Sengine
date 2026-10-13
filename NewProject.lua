newoption { trigger = "name",     value = "NAME", description = "Name of the new game project" }
newoption { trigger = "location", value = "PATH", description = "Create a standalone project in this folder" }

local ENGINE_ROOT = path.getdirectory(_SCRIPT)
local TEMPLATES   = ENGINE_ROOT .. "/Sengine/Templates"
local BINARY      = { png = true, jpg = true, wav = true, ogg = true, mp3 = true, ttf = true }

local function run(command, optional)
	print("> " .. command)
	-- cmd.exe strips one pair of outer quotes, so wrap the whole command in another pair.
	local shellCommand = os.ishost("windows") and ('"' .. command .. '"') or command
	local ok = os.execute(shellCommand)
	if not ok and not optional then error("Command failed: " .. command) end
	return ok
end

local function copyTemplate(src, dst, name)
	if not os.isdir(src) then error("Template folder not found: " .. src) end
	local files = os.matchfiles(src .. "/**")
	if #files == 0 then error("Template folder is empty: " .. src) end

	for _, file in ipairs(files) do
		local rel = (path.getrelative(src, file):gsub("__PROJECT__", name))
		local out = path.join(dst, rel)

		local ok, err = os.mkdir(path.getdirectory(out))
		if not ok then error("Could not create " .. path.getdirectory(out) .. ": " .. tostring(err)) end

		local ext = path.getextension(file):sub(2):lower()
		if BINARY[ext] then
			if not os.copyfile(file, out) then error("Could not copy " .. file) end
		else
			if not io.writefile(out, (io.readfile(file):gsub("__PROJECT__", name))) then
				error("Could not write " .. out)
			end
		end
		print("  " .. rel)
	end
end

newaction {
	trigger = "newproject",
	description = "Create a new game project from the Sengine templates",
	execute = function()
		local name = _OPTIONS["name"]
		if not name or not name:match("^[%a_][%w_]*$") then
			error("Usage: premake5 newproject --name=Rocket [--location=C:/Users/You/Documents]")
		end

		local location = _OPTIONS["location"]

		-- In-engine project under Games/ (for engine development)
		if not location then
			local dst = ENGINE_ROOT .. "/Games/" .. name
			if os.isdir(dst) then error(dst .. " already exists") end
			copyTemplate(TEMPLATES .. "/Game", dst, name)
			print("Created " .. dst)
			return
		end

		-- Standalone project. It contains no engine: it builds against the shared install in SENGINE_ROOT.
		local root = path.join(path.getabsolute(location), name)
		if os.isdir(root) then error(root .. " already exists") end
		if not os.mkdir(root) then error("Could not create " .. root) end

		copyTemplate(TEMPLATES .. "/Workspace", root, name)
		copyTemplate(TEMPLATES .. "/Game", root .. "/" .. name, name)
		io.writefile(root .. "/.gitignore", "bin/\nbin-int/\n.vs/\n*.sln\n*.vcxproj*\nMakefile\n*.make\n")

		-- A git repo for the game is handy but not required.
		if not run('git -C "' .. root .. '" init --quiet', true) then
			print("Warning: git not found, skipped 'git init'.")
		end

		-- Pass --engine explicitly: SENGINE_ROOT may not be in this process's environment yet.
		local generator = os.ishost("windows") and "vs2022" or "gmake2"
		run('"' .. _PREMAKE_COMMAND .. '" --file="' .. root .. '/premake5.lua" --engine="' .. ENGINE_ROOT .. '" ' .. generator)

		print("Created " .. root)
	end
}
