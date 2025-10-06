project "Box2D"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	defines { "_Static_assert=static_assert" }

	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)

	files {
		"Module/src/**.cpp",
		"Module/include/**.h"
	}

	includedirs {
		"Module/include"
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