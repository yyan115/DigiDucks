@echo off
echo Initializing Git submodules...
git submodule init
git submodule update --recursive --remote

echo Running Premake5 to generate project files...
premake5 vs2022

echo All submodules updated and project files generated successfully.
pause
