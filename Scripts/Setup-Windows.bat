@echo off

:: Data
set "PREMAKE_PATH=Vendor\Premake\Windows\premake5.exe"
set "PREMAKE_PATH_BASH=Vendor/Premake/Windows/premake5.exe"
set "HOOK_PATH=../.git/hooks/post-checkout"
set "BOX2D_SUBMODULE_PATH=../Vendor/Box2D/"


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
	echo %PREMAKE_PATH_BASH% --file=Build.lua vs2022
) > %HOOK_PATH%
echo Hook installed at %HOOK_PATH%


:: Update submodules
git submodule update --init --recursive

:: Update Box2D Submodule to v2.4.2
echo Updating Box2D submodule to v2.4.2...
pushd "%BOX2D_SUBMODULE_PATH%"
git checkout v2.4.2
popd
echo Box2D submodule updated to v2.4.2!

pause