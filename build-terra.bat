@echo off
setlocal

cmake -S . -B build -G "Visual Studio 17 2022" -A x64
if errorlevel 1 (
    echo Configure failed.
    pause
    exit /b 1
)

cmake --build build --config Debug
if errorlevel 1 (
    echo Build failed.
    pause
    exit /b 1
)

echo Build succeeded.
pause