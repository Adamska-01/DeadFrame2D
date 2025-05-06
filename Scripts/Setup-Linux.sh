#!/bin/bash

set -e

cd .. 

# Config
PREMAKE_PATH="Vendor/Premake/Linux/premake5"
HOOK_PATH=".git/hooks/post-checkout"
SDL_AUTOMATION_SCRIPT_PATH="Vendor/SDL/Linux_SDL_Libs_Automation.sh"


# Run Premake
echo Generating project files...
chmod +x "$PREMAKE_PATH"
$PREMAKE_PATH --cc=clang --file=Build.lua gmake2
chmod -x "$PREMAKE_PATH"
echo Project files generated!


# Install Git post-checkout hook
cat > "$HOOK_PATH" << EOF
#!/bin/sh
echo "Regenerating project files with Premake..."
chmod +x "$PREMAKE_PATH"
$PREMAKE_PATH --cc=clang --file=Build.lua gmake2
chmod -x "$PREMAKE_PATH"
EOF
chmod +x "$HOOK_PATH"
echo "Hook installed at $HOOK_PATH"


# Install SDL Libs 
echo "Building SDL Libs..."
chmod +x $SDL_AUTOMATION_SCRIPT_PATH
bash "$SDL_AUTOMATION_SCRIPT_PATH"


# Update submodules
git submodule update --init --recursive

# Update Box2D Submodule to v2.4.2
echo "Updating Box2D submodule to v2.4.2..."
pushd "$BOX2D_SUBMODULE_PATH"
git checkout v2.4.2
popd
echo "Box2D submodule updated to v2.4.2!"