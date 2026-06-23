@echo off
setlocal

echo Select build target:
echo   1. ScuffedMon
echo   2. Game
echo   3. All
echo.
set /p TARGET_CHOICE=Build target [1-3]:

set "BUILD_TARGET="
set "BUILD_LABEL=All"

if "%TARGET_CHOICE%"=="1" (
    set "BUILD_TARGET=ScuffedMon"
    set "BUILD_LABEL=ScuffedMon"
    goto target_selected
)

if "%TARGET_CHOICE%"=="2" (
    set "BUILD_TARGET=Game"
    set "BUILD_LABEL=Game"
    goto target_selected
)

if "%TARGET_CHOICE%"=="3" goto target_selected

echo Invalid build target.
pause
exit /b 1

:target_selected

echo Select build config:
echo   1. Debug
echo   2. Release
echo.
set /p CONFIG_CHOICE=Build config [1-2]:

set "CONFIG_TARGET=Debug"

if "%CONFIG_CHOICE%"=="1" (
    goto config_selected
)

if "%CONFIG_CHOICE%"=="2" (
    set "CONFIG_TARGET=Release"
    goto config_selected
)

echo Invalid build config.
pause
exit /b 1

:config_selected
cmake -S . -B bin\cmake -G "Visual Studio 17 2022" -A x64
if errorlevel 1 (
    echo Configure failed.
    pause
    exit /b 1
)

if "%BUILD_TARGET%"=="" (
    cmake --build bin\cmake --config %CONFIG_TARGET%
) else (
    cmake --build bin\cmake --config %CONFIG_TARGET% --target %BUILD_TARGET%
)
if errorlevel 1 (
    echo Build failed.
    pause
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -Command "$ws = New-Object -ComObject WScript.Shell; $shortcut = $ws.CreateShortcut((Join-Path (Get-Location) 'Terra.sln.lnk')); $shortcut.TargetPath = (Join-Path (Get-Location) 'bin\cmake\Terra.sln'); $shortcut.WorkingDirectory = (Join-Path (Get-Location) 'bin\cmake'); $shortcut.Save()"
if errorlevel 1 (
    echo Shortcut creation failed.
    pause
    exit /b 1
)

echo %BUILD_LABEL% build succeeded.
pause
