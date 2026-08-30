-- RmlUi static library (Core + Debugger), pinned to release 6.3.
--
-- Only the Core and Debugger modules are built. The Lua, Lottie and SVG plugins are deliberately left
-- out: they pull extra third-party dependencies and the engine exposes none of their functionality.
--
-- RMLUI_STATIC_LIB is PUBLIC in RmlUi's own CMake -- every consumer must define it too, otherwise the
-- public headers fall back to __declspec(dllimport). DeadFrame2D defines it in Build-DeadFrame2D.lua.
project "RmlUi"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)

	dependson { "FreeType" }

	defines {
		"RMLUI_STATIC_LIB",
		"RMLUI_VERSION=\"6.3\"",
		-- Compiles in the default FreeType font engine AND registers it at Initialise. Without this the
		-- FontEngineDefault sources still build, but no font engine is installed and Initialise fails.
		"RMLUI_FONT_ENGINE_FREETYPE"
	}

	files {
		"Module/Include/RmlUi/Config/**.h",
		"Module/Include/RmlUi/Core/**.h",
		"Module/Include/RmlUi/Core.h",
		"Module/Include/RmlUi/Debugger/**.h",
		"Module/Include/RmlUi/Debugger.h",
		"Module/Source/Core/**.cpp",
		"Module/Source/Core/**.h",
		"Module/Source/Core/**.inl",
		"Module/Source/Debugger/**.cpp",
		"Module/Source/Debugger/**.h"
	}

	includedirs {
		"Module/Include",
		"../FreeType/Module/include"
	}


	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x64"


	filter "system:windows"
		defines { "_CRT_SECURE_NO_WARNINGS" }


	filter {}
	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		runtime "Release"
		optimize "On"
		symbols "Off"