cmake_minimum_required(VERSION 3.21)

project(QuackKitchen VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(QUACK_KITCHEN_WARNINGS_AS_ERRORS "Treat first-party warnings as errors" OFF)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

find_package(glfw3 CONFIG QUIET)
if(TARGET glfw)
    set(QUACK_KITCHEN_GLFW_TARGET glfw)
elseif(TARGET glfw3::glfw)
    set(QUACK_KITCHEN_GLFW_TARGET glfw3::glfw)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW3 REQUIRED IMPORTED_TARGET glfw3)
    set(QUACK_KITCHEN_GLFW_TARGET PkgConfig::GLFW3)
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
        IMPORTED_LOCATION "${QUACK_KITCHEN_FMOD_ROOT}/lib/x64/libfmod.so.14.9"
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
    PRIVATE DUCKENGINE_EXPORTS
    PUBLIC $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:QUACK_KITCHEN_NON_MSVC>)

target_include_directories(DuckEngine
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/include"
        "${QUACK_KITCHEN_FMOD_ROOT}/inc"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/include/imgui"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/lib/glm"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/lib/stb-image"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/lib/nlohmannjson")

target_link_libraries(DuckEngine
    PUBLIC QuackKitchen::FMOD
    PRIVATE
        OpenGL::GL
        Freetype::Freetype
        GLEW::GLEW
        ${QUACK_KITCHEN_GLFW_TARGET})

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

if(BUILD_TESTING)
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
