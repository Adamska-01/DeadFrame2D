project "tinyxml2"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off" -- TODO: Turn this on for every build file

	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)

	files {
		"tinyxml2.cpp",
		"tinyxml2.h"
	}

	includedirs {
		"./"
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