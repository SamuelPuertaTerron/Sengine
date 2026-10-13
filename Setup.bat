@echo off
:: Registers this folder as the Sengine install, like Unreal's Setup.bat.
:: Game projects find the engine through SENGINE_ROOT.
set "ROOT=%~dp0"
set "ROOT=%ROOT:~0,-1%"

setx SENGINE_ROOT "%ROOT%" >nul
if errorlevel 1 (
	echo Failed to set SENGINE_ROOT.
	pause
	exit /b 1
)

echo SENGINE_ROOT set to %ROOT%
echo Restart Visual Studio and any open terminals so they pick it up.
pause
