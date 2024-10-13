@echo off
cd /d "%~dp0"

set "exeName=PingTracer.exe"

for %%I in ("%exeName%") do set "appName=%%~nI"
set "installFolder=%APPDATA%\%appName%"

echo This program will uninstall %appName% from your system.
choice /C YN /M "Do you want to continue?"
if errorlevel 2 goto :exit
cls

if not exist "%installFolder%" (
    echo Error: %appName% installation not found in %installFolder%.
    goto :exit
)

echo Removing installation folder...
rmdir /s /q "%installFolder%"

echo Removing shortcuts...
if exist "%USERPROFILE%\Desktop\%appName%.lnk" del "%USERPROFILE%\Desktop\%appName%.lnk"
rd /s /q "%APPDATA%\Microsoft\Windows\Start Menu\Programs\%appName%\"

echo %appName% has been successfully uninstalled.
pause
goto :eof

:exit
echo %appName% uninstallation cancelled or encountered an error.
pause