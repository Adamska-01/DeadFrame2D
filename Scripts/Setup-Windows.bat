@echo off

:: Data
set "PREMAKE_PATH=Vendor\Premake\Windows\premake5.exe"
set "HOOK_PATH=..\.git\hooks\post-checkout"


:: Run Premake

echo Generating project files...
pushd ..
%PREMAKE_PATH% --file=Build.lua vs2022
popd
echo Project files generated!


:: Install Git hook (post-checkout)
echo Installing Git post-checkout hook...
(
	echo #!/bin/sh
	echo echo "Regenerating project files with Premake..."
	echo %PREMAKE_PATH% --file=Build.lua vs2022
) > %HOOK_PATH%
echo Hook installed at %HOOK_PATH%

pause