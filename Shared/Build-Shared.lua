project "Shared"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)
	debugdir "../"

	defines { "SHARED_DYNAMIC", "SHARED_DYNAMIC_BUILD" }

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