project "tinyxml2"
	kind "StaticLib"
	location "./tinyxml2-10.0.0"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("./Binaries/tinyxml2-10.0.0/" .. OutputDir)
	objdir ("./Binaries/tinyxml2-10.0.0/Intermediates/" .. OutputDir)

	files {
		"tinyxml2-10.0.0/tinyxml2.cpp",
		"tinyxml2-10.0.0/tinyxml2.h"
	}

	includedirs {
		"tinyxml2-10.0.0"
	}

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