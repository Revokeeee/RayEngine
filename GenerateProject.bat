@echo off
echo === RayEngineProject CMake Build Script ===

if exist build (
    echo Removing old build directory...
    rmdir /S /Q build
)

mkdir build
cd build

echo Generating Visual Studio solution...
cmake -G "Visual Studio 17 2022" -A x64 ..


cd ..
pause
