@echo off
cd /d "%~dp0"

set "exeName=PingPlotter.exe"
set "config1=PingPlotterConfig.ini"

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
set "sourceFolder=%~dp0%appName%"
set "installFolder=%ProgramFiles%\%appName%"

echo This program will install %appName% on your system.
choice /C YN /M "Do you want to continue?"
if errorlevel 2 goto :exit
cls

if not exist "%sourceFolder%" (
    echo Error: Source folder %sourceFolder% not found.
    goto :exit
)
if not exist "%sourceFolder%\%exeName%" (
    echo Error: %exeName% not found in the source folder.
    goto :exit
)
if not exist "%sourceFolder%\%config1%" (
    echo Error: %config1% not found in the source folder.
    goto :exit
)

if exist "%installFolder%" rmdir /s /q "%installFolder%"
mkdir "%installFolder%"

echo Creating AppData folder...
mkdir "%APPDATA%\%appName%"

echo Creating Documents folder...
mkdir "%USERPROFILE%\Documents\%appName%"

echo Copying files...
xcopy "%sourceFolder%\*" "%installFolder%\" /E /I /H /Y >nul

echo Creating shortcuts...
powershell -Command "$ws = New-Object -ComObject WScript.Shell; $s = $ws.CreateShortcut('%USERPROFILE%\Desktop\%appName%.lnk'); $s.TargetPath = '%installFolder%\%exeName%'; $s.WorkingDirectory = '%installFolder%'; $s.Save()"
powershell -Command "$ws = New-Object -ComObject WScript.Shell; $s = $ws.CreateShortcut('%APPDATA%\Microsoft\Windows\Start Menu\Programs\%appName%.lnk'); $s.TargetPath = '%installFolder%\%exeName%'; $s.WorkingDirectory = '%installFolder%'; $s.Save()"

echo %appName% installation completed successfully!
echo Please feel free to delete the %appName% folder, or keep it as a portable version
pause
goto :eof

:exit
echo %appName% installation cancelled or encountered an error.
pause