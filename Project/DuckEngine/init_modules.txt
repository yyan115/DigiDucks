@echo off
echo ------------------------------------------------------------
echo Setting up the project environment...
echo ------------------------------------------------------------

:: Define paths for Git and Premake5
set "GIT_PATH=Engine\tools\PortableGit\bin\git.exe"
set "PREMAKE5_PATH=Engine\tools\premake5.exe"
set "VENDOR_DIR=Engine\lib"

:: Check if Git exists locally in the tools folder
if exist "%GIT_PATH%" (
    echo Using portable Git at %GIT_PATH%
) else (
    echo Git not found. Please install Git or include it in the "tools" directory.
    pause
    exit /b
)

:: Check if Premake5 exists locally in the tools folder
if exist "%PREMAKE5_PATH%" (
    echo Using Premake5 at %PREMAKE5_PATH%
) else (
    echo Premake5 not found. Please install Premake5 or include it in the "tools" directory.
    pause
    exit /b
)

:: Clone the repositories if not already present
echo Cloning libraries...
if not exist "%VENDOR_DIR%\GLFW" (
    "%GIT_PATH%" clone https://github.com/TheCherno/GLFW "%VENDOR_DIR%\GLFW"
) else (
    echo GLFW already exists.
)

if not exist "%VENDOR_DIR%\spdlog" (
    "%GIT_PATH%" clone https://github.com/gabime/spdlog "%VENDOR_DIR%\spdlog"
) else (
    echo spdlog already exists.
)

if not exist "%VENDOR_DIR%\glm" (
    "%GIT_PATH%" clone https://github.com/g-truc/glm "%VENDOR_DIR%\glm"
) else (
    echo glm already exists.
)

if not exist "%VENDOR_DIR%\rapidjson" (
    "%GIT_PATH%" clone https://github.com/miloyip/rapidjson "%VENDOR_DIR%\rapidjson"
) else (
    echo rapidjson already exists.
)

if not exist "%VENDOR_DIR%\imgui" (
    "%GIT_PATH%" clone https://github.com/ocornut/imgui "%VENDOR_DIR%\imgui"
) else (
    echo imgui already exists.
)

:: Run Premake5 to generate project files
echo Running Premake5 to generate project files...
"%PREMAKE5_PATH%" vs2022  (or your specific platform)

:: Check if the project files were created successfully
if exist "Engine/YourProject.sln" (
    echo Project files generated successfully.
) else (
    echo Error: Project files were not generated.
    pause
    exit /b
)

echo ------------------------------------------------------------
echo Setup complete. You can now open the project in your IDE.
echo ------------------------------------------------------------
pause
