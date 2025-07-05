#!/bin/bash
set -e

cd "$(dirname "$(realpath "$0")")"

mkdir repos
cd repos

# Data
BUILD_ROOT="$(pwd)"
BUILD_TMP="${BUILD_ROOT}/SDL_build_tmp"
TEMP_OUTPUT="${BUILD_ROOT}/output_temp"

declare -A SDL_LIBS
SDL_LIBS[SDL]=2.30.10
SDL_LIBS[SDL_image]=2.8.2
SDL_LIBS[SDL_mixer]=2.8.0
SDL_LIBS[SDL_ttf]=2.22.0

declare -A LIB_ROOT_DESTINATION
LIB_ROOT_DESTINATION[SDL]="SDL2-${SDL_LIBS[SDL]}"
LIB_ROOT_DESTINATION[SDL_image]="SDL2_image-${SDL_LIBS[SDL_image]}"
LIB_ROOT_DESTINATION[SDL_mixer]="SDL2_mixer-${SDL_LIBS[SDL_mixer]}"
LIB_ROOT_DESTINATION[SDL_ttf]="SDL2_ttf-${SDL_LIBS[SDL_ttf]}"

declare -A SDL_LIB_PREFIXES
SDL_LIB_PREFIXES[SDL]="libSDL2"
SDL_LIB_PREFIXES[SDL_image]="libSDL2_image"
SDL_LIB_PREFIXES[SDL_mixer]="libSDL2_mixer"
SDL_LIB_PREFIXES[SDL_ttf]="libSDL2_ttf"


prompt_install_dependencies() {
	echo "❓ Install required system packages (build tools, freetype, etc)? [y/N]"
	read -r answer
	if [[ "$answer" =~ ^[Yy]$ ]]; then
		sudo apt update
		sudo apt install -y build-essential gcc-multilib g++-multilib pkg-config autoconf automake libtool
		sudo dpkg --add-architecture i386
		sudo apt update
		sudo apt install -y libfreetype6-dev libfreetype6-dev:i386
	else
		echo "⚠️  Skipping dependency installation. Make sure they're installed!"
	fi
}

clone_repo() {
	local name=$1 version=$2
	local dir_name="${name}-release-${version}"

	if [ ! -d "$dir_name" ]; then
		echo "📥 Cloning ${name} (${version})..."
		git clone --depth 1 --recurse-submodules --branch release-${version} "https://github.com/libsdl-org/${name}.git" "$dir_name"
	else
		echo "📦 ${name} already cloned."
	fi
}

build_library() {
	local name=$1 arch=$2 cflags=$3
	local src_dir="${name}-release-${SDL_LIBS[$name]}"
	local build_dir="${BUILD_TMP}/${name}/${arch}"
	local install_dir="${TEMP_OUTPUT}/${name}/${arch}"
	local output_dir="${BUILD_ROOT}/../${LIB_ROOT_DESTINATION[$name]}/lib/Linux/${arch}"
	
	echo "🔧 Building ${LIB_ROOT_DESTINATION[${name}]} for ${arch}..."
	mkdir -p "$build_dir" "$output_dir"
	pushd "$build_dir" > /dev/null

	local sdl_prefix="${TEMP_OUTPUT}/SDL/${arch}"
	local pkg_config_path=""
	local sdl2_config="${sdl_prefix}/bin/sdl2-config"

	# Set pkg-config path and SDL2_CONFIG for non-SDL libs
	if [ "$name" != "SDL" ]; then
		export SDL2_CONFIG="$sdl2_config"
		export PATH="$(dirname "$sdl2_config"):$PATH"
		export PKG_CONFIG_PATH="${sdl_prefix}/lib/pkgconfig"

		echo "🔎 Verifying sdl2-config: $SDL2_CONFIG"
		"$SDL2_CONFIG" --version || {
			echo "❌ sdl2-config not found or not working"
			exit 1
		}
	fi
	
	 # Run autogen.sh if it exists
    if [ -f "${BUILD_ROOT}/${src_dir}/autogen.sh" ]; then
        echo "⚙️ Running autogen.sh for ${name}..."
        pushd "${BUILD_ROOT}/${src_dir}" > /dev/null
        ./autogen.sh
        popd > /dev/null
    fi
	
	# Special handling for SDL_ttf to avoid harfbuzz and link issues
	local extra_args=""
	local extra_ldflags=""
	if [ "$name" == "SDL_ttf" ]; then
		extra_args="--enable-freetype-builtin=no --disable-harfbuzz"
		extra_ldflags="-L/usr/lib/${arch}-linux-gnu"
		pkg_config_path="/usr/lib/${arch}-linux-gnu/pkgconfig:/usr/share/pkgconfig:/usr/lib/pkgconfig:$pkg_config_path"
	fi

	PKG_CONFIG_PATH="$pkg_config_path" \
	CFLAGS="$cflags" LDFLAGS="$cflags $extra_ldflags" \
		"${BUILD_ROOT}/${src_dir}/configure" --prefix="$install_dir" $extra_args
	
	make -j"$(nproc)"
	make install

	local lib_prefix="${SDL_LIB_PREFIXES[$name]}"
	find "${install_dir}/lib" -name "${lib_prefix}.so*" -exec cp {} "$output_dir/" \;

	ldd "$output_dir/${lib_prefix}.so" || echo "⚠️  Could not verify shared libs"
	
	if [ "$name" != "SDL" ]; then
		unset SDL2_CONFIG
		unset PKG_CONFIG_PATH
	fi

	popd > /dev/null
	echo "✅ ${name} ${arch} build complete"
}


main() {
	prompt_install_dependencies
	
	mkdir -p "$BUILD_TMP" "$TEMP_OUTPUT"

	# Clone all
	for lib in "${!SDL_LIBS[@]}"; do
		clone_repo "$lib" "${SDL_LIBS[$lib]}"
	done

	# Build all (x86 + x64)
	for arch in x86 x64; do
		cflags=$([ "$arch" == "x86" ] && echo "-m32" || echo "-m64")
		# This sucks, but SDL must be built first
		for lib in SDL SDL_image SDL_mixer SDL_ttf; do
			build_library "$lib" "$arch" "$cflags"
		done
	done

	echo "🧹 Cleaning up..."
	cd ..
	rm -rf repos
}

main