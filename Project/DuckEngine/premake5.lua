workspace "DuckEngine"
   configurations { "Debug", "Release" }

project "DuckEngine"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { "src/**.h", "src/**.cpp" }

   includedirs {
      "Engine/lib/GLFW/include",
      "Engine/lib/spdlog/include",
      "Engine/lib/glm",
      "Engine/lib/rapidjson/include",
      "Engine/lib/imgui"
   }

   libdirs {
      "Engine/lib/GLFW/lib",
      "Engine/lib/spdlog/lib",
      -- Add any other necessary libdirs
   }

   links { "GLFW", "spdlog", "glm", "rapidjson", "imgui" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
