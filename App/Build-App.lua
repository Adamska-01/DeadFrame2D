project "App"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	targetdir "Binaries/%{cfg.buildcfg}"
	staticruntime "off"

	files 
	{ 
		"include/**.h", 
		"src/**.cpp", 
		"../Vendor/tinyxml2-10.0.0/**.cpp" 
	}

	includedirs
	{
		-- Include Core
		"include",
		"../Core/2DGameEngine/include",
        "../Vendor/tinyxml2-10.0.0", 
		-- There are a lot of useful sdl structs/classes that the app could use
		"../Vendor/SDL/SDL2-2.30.10/include",
		"../Vendor/SDL/SDL2_image-2.8.2/include",
		"../Vendor/SDL/SDL2_ttf-2.22.0/include",
		"../Vendor/SDL/SDL2_mixer-2.8.0/include"
	}

	links
	{
		"2DGameEngine"
	}

	-- Copy entire Assets folder in the target dir
    postbuildcommands
    {
        '{COPY} "./Assets/" "%{cfg.targetdir}/Assets/"'
    }

	-- Ensure the app depends on the 2DGameEngine project
	dependson { "2DGameEngine" }
	
	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)

	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x64"