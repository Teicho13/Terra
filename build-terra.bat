@echo off
setlocal

cmake -S . -B bin\cmake -G "Visual Studio 17 2022" -A x64
if errorlevel 1 (
    echo Configure failed.
    pause
    exit /b 1
)

cmake --build bin\cmake --config Debug
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

echo Build succeeded.
pause