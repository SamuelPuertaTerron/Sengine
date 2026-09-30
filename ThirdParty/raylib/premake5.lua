project "raylib"
    kind "StaticLib"
    language "C"
    
    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"
    
    files 
    {
        "src/rcore.c",
        "src/rshapes.c",
        "src/rtextures.c",
        "src/rtext.c",
        "src/rmodels.c",
        "src/utils.c",
        "src/raudio.c",
        "src/rglfw.c",
        "src/raygui.c",
        
        "include/**.h",
    }
    
    includedirs 
    {
        "src",
        "src/external/glfw/include",
        "src/external/glfw/deps/mingw",
        "include",
    }
    
    defines 
    {
        "GRAPHICS_API_OPENGL_33",
        "PLATFORM_RGFW",
        "USE_RGFW",
        "PLATFORM_DESKTOP",
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_WARNINGS",
        "WIN32_LEAN_AND_MEAN",  -- Reduces Windows.h includes
        "NOMINMAX",             -- Prevents min/max macros
        -- Prevent Windows API conflicts
        "CloseWindow=raylib_CloseWindow",
        "ShowCursor=raylib_ShowCursor"
    }
    
    filter "system:windows"
        defines { 
            "_WIN32",
            "NOMINMAX",  -- Prevent min/max macros
            "_CRT_SECURE_NO_WARNINGS"
        }
        files { "src/rglfw.c" }
        links { "winmm", "gdi32", "opengl32" }
        buildoptions { "/wd4267" }  -- Disable size_t to int conversion warnings
        disablewarnings { "4267", "4006" }  -- Disable specific warnings

    filter "system:linux"
        defines { "_GLFW_X11" }
        links { "pthread", "m", "dl", "X11" }
        buildoptions { "-std=gnu99", "-Wno-conversion" }

    filter "configurations:Debug"
        defines { "_DEBUG" }

    filter "configurations:Release"
        defines { "NDEBUG" }