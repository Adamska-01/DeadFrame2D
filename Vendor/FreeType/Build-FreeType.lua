-- FreeType static library (VER-2-14-3).
--
-- The source list mirrors BASE_SRCS in Module/CMakeLists.txt (VER-2-14-3). FreeType compiles each
-- module through a single amalgamation .c file that #includes its parts, so the list is short and
-- must be explicit -- a "src/**.c" glob would pull the parts in a second time and fail to link.
--
-- No optional dependency is enabled (zlib, bzip2, png, harfbuzz, brotli all stay off in
-- Module/include/freetype/config/ftoption.h), so this project builds standalone.
project "FreeType"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)

	defines { "FT2_BUILD_LIBRARY" }

	files {
		"Module/src/autofit/autofit.c",
		"Module/src/base/ftbase.c",
		"Module/src/base/ftbbox.c",
		"Module/src/base/ftbdf.c",
		"Module/src/base/ftbitmap.c",
		"Module/src/base/ftcid.c",
		"Module/src/base/ftfstype.c",
		"Module/src/base/ftgasp.c",
		"Module/src/base/ftglyph.c",
		"Module/src/base/ftgxval.c",
		"Module/src/base/ftinit.c",
		"Module/src/base/ftmm.c",
		"Module/src/base/ftotval.c",
		"Module/src/base/ftpatent.c",
		"Module/src/base/ftpfr.c",
		"Module/src/base/ftstroke.c",
		"Module/src/base/ftsynth.c",
		"Module/src/base/fttype1.c",
		"Module/src/base/ftwinfnt.c",
		"Module/src/bdf/bdf.c",
		"Module/src/bzip2/ftbzip2.c",
		"Module/src/cache/ftcache.c",
		"Module/src/cff/cff.c",
		"Module/src/cid/type1cid.c",
		"Module/src/gzip/ftgzip.c",
		"Module/src/lzw/ftlzw.c",
		"Module/src/pcf/pcf.c",
		"Module/src/pfr/pfr.c",
		"Module/src/psaux/psaux.c",
		"Module/src/pshinter/pshinter.c",
		"Module/src/psnames/psnames.c",
		"Module/src/raster/raster.c",
		"Module/src/sdf/sdf.c",
		"Module/src/sfnt/sfnt.c",
		"Module/src/smooth/smooth.c",
		"Module/src/svg/svg.c",
		"Module/src/truetype/truetype.c",
		"Module/src/type1/type1.c",
		"Module/src/type42/type42.c",
		"Module/src/winfonts/winfnt.c",
		"Module/include/ft2build.h",
		"Module/include/freetype/**.h"
	}

	includedirs {
		"Module/include"
	}


	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x64"


	-- ftsystem/ftdebug have platform-specific implementations; CMake picks them the same way.
	filter "system:windows"
		files {
			"Module/builds/windows/ftsystem.c",
			"Module/builds/windows/ftdebug.c"
		}
		defines { "_CRT_SECURE_NO_WARNINGS" }

	filter "system:linux"
		files {
			"Module/src/base/ftsystem.c",
			"Module/src/base/ftdebug.c"
		}


	filter {}
	filter "configurations:Debug"
		defines { "DEBUG", "FT_DEBUG_LEVEL_ERROR" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		runtime "Release"
		optimize "On"
		symbols "Off"