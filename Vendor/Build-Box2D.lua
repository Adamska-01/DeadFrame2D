project "Box2D"
	kind "StaticLib"
	location "./Box2D"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	defines { "_Static_assert=static_assert" }

	targetdir ("./Binaries/Box2D/" .. OutputDir)
	objdir ("./Binaries/Box2D/Intermediates/" .. OutputDir)

	files {
		"Box2D/src/**.cpp",
		"Box2D/include/**.h"
	}

	includedirs {
		"Box2D/include"
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