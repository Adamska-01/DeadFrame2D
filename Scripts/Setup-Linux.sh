#!/bin/bash

set -e

# Config
PREMAKE_PATH="Vendor/Premake/Linux/premake5"
HOOK_PATH="../.git/hooks/post-checkout"


# Run Premake
echo Generating project files...
pushd ..
$PREMAKE_PATH --cc=clang --file=Build.lua gmake2
popd
echo Project files generated!


# Install Git post-checkout hook
echo "Installing Git post-checkout hook..."
cat > "$HOOK_PATH" << 'EOF'
#!/bin/sh
echo "Regenerating project files with Premake..."
$PREMAKE_PATH --cc=clang --file=Build.lua gmake2
EOF
chmod +x "$HOOK_PATH"
echo "Hook installed at $HOOK_PATH"