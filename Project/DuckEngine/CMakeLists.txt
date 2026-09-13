cmake_minimum_required(VERSION 3.21)

project(QuackKitchen VERSION 0.1.2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(QUACK_KITCHEN_WARNINGS_AS_ERRORS "Treat first-party warnings as errors" OFF)
# Read-only sampler used to drive the game from outside it. Off by default,
# so no release package contains it. Even when on it stays inert unless
# QUACK_KITCHEN_TELEMETRY_PATH names a file.
option(QUACK_KITCHEN_TELEMETRY "Compile the read-only telemetry sampler" OFF)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

# GLFW_CURSOR_CAPTURED, which keeps the cursor inside the window, arrived in
# GLFW 3.4. Ubuntu 24.04 still ships 3.3.10, so asking for any GLFW at all
# produces a release build where the cursor is not confined and nothing says
# so. Ask for 3.4, and build the copy in Engine/lib/glfw when the system has
# nothing new enough.
set(QUACK_KITCHEN_GLFW_MINIMUM 3.4)
option(QUACK_KITCHEN_BUNDLED_GLFW
    "Build the bundled GLFW instead of looking for a system one" OFF)

if(NOT QUACK_KITCHEN_BUNDLED_GLFW)
    find_package(glfw3 ${QUACK_KITCHEN_GLFW_MINIMUM} CONFIG QUIET)
    if(TARGET glfw)
        set(QUACK_KITCHEN_GLFW_TARGET glfw)
    elseif(TARGET glfw3::glfw)
        set(QUACK_KITCHEN_GLFW_TARGET glfw3::glfw)
    else()
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(GLFW3 QUIET IMPORTED_TARGET
                "glfw3 >= ${QUACK_KITCHEN_GLFW_MINIMUM}")
        endif()
        if(TARGET PkgConfig::GLFW3)
            set(QUACK_KITCHEN_GLFW_TARGET PkgConfig::GLFW3)
        endif()
    endif()
endif()

if(NOT QUACK_KITCHEN_GLFW_TARGET)
    message(STATUS
        "No GLFW ${QUACK_KITCHEN_GLFW_MINIMUM} or newer found; building the bundled copy")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    if(UNIX AND NOT APPLE)
        # X11 only. Wayland needs wayland-scanner and a pile of protocol
        # headers on the build machine, and every session the game is driven
        # in is X11 or XWayland.
        set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
    endif()
    add_subdirectory(Engine/lib/glfw EXCLUDE_FROM_ALL)
    set(QUACK_KITCHEN_GLFW_TARGET glfw)
endif()

set(QUACK_KITCHEN_FMOD_ROOT
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/lib/FMOD-2.03.09/core")

add_library(QuackKitchenFMOD SHARED IMPORTED GLOBAL)
add_library(QuackKitchen::FMOD ALIAS QuackKitchenFMOD)

if(WIN32)
    set_target_properties(QuackKitchenFMOD PROPERTIES
        IMPORTED_CONFIGURATIONS "Debug;Release;RelWithDebInfo;MinSizeRel"
        IMPORTED_IMPLIB_DEBUG "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/fmodL_vc.lib"
        IMPORTED_LOCATION_DEBUG "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/fmodL.dll"
        IMPORTED_IMPLIB_RELEASE "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/fmod_vc.lib"
        IMPORTED_LOCATION_RELEASE "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/fmod.dll"
        IMPORTED_IMPLIB_RELWITHDEBINFO "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/fmod_vc.lib"
        IMPORTED_LOCATION_RELWITHDEBINFO "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/fmod.dll"
        IMPORTED_IMPLIB_MINSIZEREL "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/fmod_vc.lib"
        IMPORTED_LOCATION_MINSIZEREL "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/fmod.dll")
elseif(UNIX AND NOT APPLE)
    set_target_properties(QuackKitchenFMOD PROPERTIES
        IMPORTED_LOCATION "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/libfmod.so.14"
        IMPORTED_SONAME "libfmod.so.14")
else()
    message(FATAL_ERROR "Quack Kitchen currently supports Windows and Linux")
endif()

file(GLOB QUACK_KITCHEN_ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/src/*.cpp")

set(QUACK_KITCHEN_IMGUI_SOURCES
    Engine/src/imgui/imgui.cpp
    Engine/src/imgui/imgui_draw.cpp
    Engine/src/imgui/imgui_tables.cpp
    Engine/src/imgui/imgui_widgets.cpp
    Engine/src/imgui/imgui_impl_glfw.cpp
    Engine/src/imgui/imgui_impl_opengl3.cpp)

add_library(DuckEngine SHARED
    ${QUACK_KITCHEN_ENGINE_SOURCES}
    ${QUACK_KITCHEN_IMGUI_SOURCES})

target_compile_definitions(DuckEngine
    PRIVATE
        DUCKENGINE_EXPORTS
        $<$<PLATFORM_ID:Windows>:BUILD_DLL>
    PUBLIC $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:QUACK_KITCHEN_NON_MSVC>)

target_include_directories(DuckEngine
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/include"
        "${QUACK_KITCHEN_FMOD_ROOT}/inc"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/lib/glm"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/include/imgui"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/lib/stb-image"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/lib/nlohmannjson")

target_link_libraries(DuckEngine
    PUBLIC
        QuackKitchen::FMOD
        Freetype::Freetype
        GLEW::GLEW
        ${QUACK_KITCHEN_GLFW_TARGET}
    PRIVATE
        OpenGL::GL)

if(MSVC)
    target_compile_options(DuckEngine PRIVATE /W4 /permissive-)
else()
    target_compile_options(DuckEngine
        PUBLIC -include "${CMAKE_CURRENT_SOURCE_DIR}/Engine/include/PlatformCompatibility.h"
        PRIVATE -Wall -Wextra -Wpedantic)
    if(QUACK_KITCHEN_WARNINGS_AS_ERRORS)
        target_compile_options(DuckEngine PRIVATE -Werror)
    endif()
endif()

file(GLOB QUACK_KITCHEN_GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Game/src/*.cpp")

if(WIN32)
    file(TO_CMAKE_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/GameIcon.ico"
        QUACK_KITCHEN_ICON_RC_PATH)
    configure_file(
        "${CMAKE_SOURCE_DIR}/packaging/windows/QuackKitchen.rc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/QuackKitchen.rc"
        @ONLY)
    add_executable(QuackKitchen WIN32
        Game/main.cpp
        ${QUACK_KITCHEN_GAME_SOURCES}
        "${CMAKE_CURRENT_BINARY_DIR}/QuackKitchen.rc")
else()
    add_executable(QuackKitchen Game/main.cpp ${QUACK_KITCHEN_GAME_SOURCES})
endif()

if(UNIX AND NOT APPLE)
    string(TIMESTAMP QUACK_KITCHEN_RELEASE_DATE "%Y-%m-%d" UTC)
    configure_file(
        "${CMAKE_SOURCE_DIR}/packaging/linux/edu.digipen.quackkitchen.metainfo.xml.in"
        "${CMAKE_CURRENT_BINARY_DIR}/edu.digipen.quackkitchen.metainfo.xml"
        @ONLY)
endif()

set_target_properties(QuackKitchen PROPERTIES
    OUTPUT_NAME "Quack Kitchen"
    BUILD_RPATH "$ORIGIN"
    INSTALL_RPATH "$ORIGIN")

set_target_properties(DuckEngine PROPERTIES
    BUILD_RPATH "$ORIGIN"
    INSTALL_RPATH "$ORIGIN")

target_include_directories(QuackKitchen PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Game/include")
target_link_libraries(QuackKitchen PRIVATE DuckEngine)
target_compile_definitions(QuackKitchen PRIVATE
    QUACK_KITCHEN_VERSION="${PROJECT_VERSION}")
if(QUACK_KITCHEN_TELEMETRY)
    target_compile_definitions(QuackKitchen PRIVATE QUACK_KITCHEN_TELEMETRY)
endif()

if(BUILD_TESTING)
	add_executable(QuackKitchenCustomerQueueTests
		Game/tests/CustomerQueuePolicyTests.cpp)
	target_include_directories(QuackKitchenCustomerQueueTests PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/Game/include")
	add_test(NAME QuackKitchen.CustomerQueuePolicy
		COMMAND QuackKitchenCustomerQueueTests)

    add_executable(QuackKitchenSaveTests
        Game/tests/SaveLoadManagerTests.cpp
        Game/src/SaveLoadManager.cpp)
    target_include_directories(QuackKitchenSaveTests PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Game/include")
    target_link_libraries(QuackKitchenSaveTests PRIVATE DuckEngine)
    add_test(NAME QuackKitchen.SaveRecovery COMMAND QuackKitchenSaveTests)
endif()

if(MSVC)
    target_compile_options(QuackKitchen PRIVATE /W4 /permissive-)
else()
    target_compile_options(QuackKitchen PRIVATE -Wall -Wextra -Wpedantic)
    if(QUACK_KITCHEN_WARNINGS_AS_ERRORS)
        target_compile_options(QuackKitchen PRIVATE -Werror)
    endif()
endif()

add_custom_command(TARGET QuackKitchen POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E remove_directory
            "$<TARGET_FILE_DIR:QuackKitchen>/Resources"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
            "$<TARGET_FILE_DIR:QuackKitchen>/Resources"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:QuackKitchenFMOD>"
            "$<TARGET_FILE_DIR:QuackKitchen>"
    COMMENT "Staging Quack Kitchen runtime resources")

install(TARGETS DuckEngine QuackKitchen
    RUNTIME DESTINATION .
    LIBRARY DESTINATION .)
install(DIRECTORY Resources DESTINATION .
    PATTERN "EditorIcons" EXCLUDE
    PATTERN "save.json" EXCLUDE)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/Licenses" DESTINATION .)

if(WIN32)
    install(FILES
        "$<$<CONFIG:Debug>:${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/fmodL.dll>"
        "$<$<NOT:$<CONFIG:Debug>>:${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/fmod.dll>"
        DESTINATION .)
elseif(UNIX AND NOT APPLE)
    install(FILES
        "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/libfmod.so.14"
        DESTINATION .)
endif()
