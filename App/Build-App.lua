project "App"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)
	dependson { "2DGameEngine", "Shared" }
	staticruntime "off"
	debugdir "../"
	
	files 
	{ 
		"include/**.h", 
		"src/**.cpp", 
	}

	includedirs
	{
		-- Include Core
		"include",
		"../Core/2DGameEngine/include",
        "../Vendor/nlohmann-3.11.3", 
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

	-- Copy Assets and Shared files in the target dir
    postbuildcommands
    {
		'{COPY} "../Shared/Configurations/" "%{cfg.targetdir}/Shared/Configurations/"',
        '{COPY} "./Assets/" "%{cfg.targetdir}/App/Assets/"'
    }

	-- Filters
	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		kind "ConsoleApp"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		runtime "Release"
		optimize "On"
		kind "WindowedApp"
		symbols "Off"

	filter "platforms:x86"
		architecture "x86"
		
	filter "platforms:x64"
		architecture "x64"