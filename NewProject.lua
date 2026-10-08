newoption { trigger = "name", value = "NAME", description = "Name of the new game project" }

newaction {
    trigger = "newproject",
    description = "Create a new game project from the Sengine template",
    execute = function()
        local name = _OPTIONS["name"]
        if not name or not name:match("^[%a_][%w_]*$") then
            error("Usage: premake5 newproject --name=Rocket  (must be a valid C++ identifier)")
        end

        local src = _MAIN_SCRIPT_DIR .. "/Sengine/Templates/Game"
        local dst = _MAIN_SCRIPT_DIR .. "/Games/" .. name
        print("Template: " .. src)

        if not os.isdir(src) then error("Template folder not found: " .. src) end
        if os.isdir(dst) then error(dst .. " already exists") end

        local files = os.matchfiles(src .. "/**")
        if #files == 0 then error("Template folder is empty: " .. src) end

        for _, file in ipairs(files) do
            local rel = (path.getrelative(src, file):gsub("__PROJECT__", name))
            local out = path.join(dst, rel)

            local ok, err = os.mkdir(path.getdirectory(out))
            if not ok then error("Could not create " .. path.getdirectory(out) .. ": " .. tostring(err)) end

            if file:match("%.png$") or file:match("%.wav$") or file:match("%.ogg$") then
                if not os.copyfile(file, out) then error("Could not copy " .. file) end
            else
                if not io.writefile(out, (io.readfile(file):gsub("__PROJECT__", name))) then
                    error("Could not write " .. out)
                end
            end
            print("  " .. rel)
        end

        print("Created " .. dst .. " (" .. #files .. " files). Run premake again to regenerate the solution.")
    end
}