project "Shared"
	kind "StaticLib"
	language "C++" -- Doesn't really matter
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)
	debugdir "../"

	files { 
		"./include/**.h",
		"./src/**.cpp", 
		"./Configurations/**.*",
		"./Resources/**.*"
	}

	includedirs {
		"include",
		"../Vendor/nlohmann-3.11.3"
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