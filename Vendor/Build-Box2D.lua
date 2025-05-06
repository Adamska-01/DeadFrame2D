project "Box2D"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	defines { "_Static_assert=static_assert" }
	targetdir ("./Box2D/Binaries/" .. OutputDir)
	objdir ("./Box2D/Binaries/Intermediates/" .. OutputDir)

	files {
		"Box2D/src/**.c",
		"Box2D/src/**.h",
		"Box2D/include/**.h"
	}

	includedirs {
		"Box2D/include"
	}