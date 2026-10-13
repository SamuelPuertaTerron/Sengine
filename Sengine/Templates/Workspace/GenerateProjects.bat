@echo off
cd /d "%~dp0"

if not defined SENGINE_ROOT (
	echo SENGINE_ROOT is not set. Install the engine from the Sengine hub, or run Setup.bat in the engine folder.
	pause
	exit /b 1
)

call "%SENGINE_ROOT%\ThirdParty\premake\premake5.exe" vs2022
pause
