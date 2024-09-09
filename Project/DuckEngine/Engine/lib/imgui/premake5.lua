project "ImGui"
    kind "StaticLib"
    staticruntime "on"
    
    language "C++"
    cppdialect "C++17"

    systemversion "latest"

  
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imconfig.h",
        "imgui.h",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_widgets.cpp",
        "imgui_tables.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "imgui_demo.cpp",
        "imgui_impl_glfw.h",
        "imgui_impl_opengl3.h",
        "imgui_impl_opengl3_loader.h",
        "imgui_impl_glfw.cpp",
        "imgui_impl_opengl3.cpp"
    }

    
    --directories to include
    includedirs
    {
        --"$(SolutionDir)extern/glfw-3.3.8.bin.WIN64/include",
        "$(SolutionDir)Duck/vendor/GLFW/include",
        "%{IncludeDir.GLFW}"
    }

    links {
		"GLFW"
    }

    filter "system:windows"
		systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
