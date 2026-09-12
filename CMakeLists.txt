cmake_minimum_required(VERSION 3.21)

project(QuackKitchenWorkspace LANGUAGES C CXX)

include(CTest)

add_subdirectory(Project/DuckEngine)

if(BUILD_TESTING)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_test(
        NAME QuackKitchen.AssetValidation
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/tools/validate_assets.py")
    set_tests_properties(QuackKitchen.AssetValidation PROPERTIES
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()
