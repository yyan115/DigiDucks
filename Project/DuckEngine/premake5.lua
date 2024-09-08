-- Define the workspace
workspace "DuckEngine"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    location "Build"
    objdir "Build/obj"
    targetdir "Build/%{cfg.buildcfg}"

-- Define the project
project "DuckEngine"
    kind "SharedLib" -- Use "StaticLib" or "ConsoleApp" as needed
    language "C++"
    targetdir "Build/%{cfg.buildcfg}/"
    location "Build"

    -- Configuration-specific settings
    filter "configurations:Debug"
        defines { "DEBUG", "_CONSOLE", "DUCKENGINE_EXPORTS", "GLEW_STATIC" }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NDEBUG", "_CONSOLE", "DUCKENGINE_EXPORTS", "GLEW_STATIC" }
        optimize "On"
        symbols "On"

    filter "platforms:x64"
        architecture "x64"

    -- Include directories
    includedirs {
        "Engine/include",
        "Engine/lib/glew/include",
        "Engine/lib/glfw/include",
        "Engine/lib/glm"
    }

    -- Library directories
    libdirs {
        "Engine/lib/glfw/",
        "Engine/lib/glew"
    }

    -- Additional dependencies
    links {
        "glfw3.lib",
        "glew32s.lib",
        "opengl32.lib"
    }

    -- Source and header files
    files {
        "src/**.cpp",
        "include/**.h"
    }

    -- Precompiled headers if needed
    -- pchheader "include/pch.h"
    -- pchsource "src/pch.cpp"
    
    -- External dependencies
    externalincludedirs {
        "Engine/lib/glew/include",
        "Engine/lib/glfw/include",
        "Engine/lib/glm"
    }
