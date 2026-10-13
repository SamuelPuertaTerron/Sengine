@echo off
cd /d "%~dp0"
call Sengine\ThirdParty\premake\premake5.exe vs2022
pause
