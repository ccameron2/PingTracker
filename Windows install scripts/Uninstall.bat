@echo off
cd /d "%~dp0"

set "exeName=PingPlotter.exe"

net session >nul 2>&1
if %errorLevel% == 0 (
    goto :admin
) else (
    echo Requesting administrative privileges...
    goto :UACPrompt
)

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    set params = %*:"=""
    echo UAC.ShellExecute "cmd.exe", "/c %~s0 %params%", "", "runas", 1 >> "%temp%\getadmin.vbs"
    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B

:admin
for %%I in ("%exeName%") do set "appName=%%~nI"
set "installFolder=%ProgramFiles%\%appName%"

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

echo Removing AppData folder...
rmdir /s /q "%APPDATA%\%appName%"

echo Removing shortcuts...
if exist "%USERPROFILE%\Desktop\%appName%.lnk" del "%USERPROFILE%\Desktop\%appName%.lnk"
if exist "%APPDATA%\Microsoft\Windows\Start Menu\Programs\%appName%.lnk" del "%APPDATA%\Microsoft\Windows\Start Menu\Programs\%appName%.lnk"

echo %appName% has been successfully uninstalled.
pause
goto :eof

:exit
echo %appName% uninstallation cancelled or encountered an error.
pause