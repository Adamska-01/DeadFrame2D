project "App"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	targetname "Application"
	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)
	dependson { "2DGameEngine", "Box2D" }
	staticruntime "on"
	debugdir "../" -- Set working directory to the root of the solution 
	

	files { 
		"include/**.h", 
		"src/**.cpp", 
	}

	includedirs {
		-- Include Core
		"include",
		"../Shared/include",
		"../Core/2DGameEngine/include",
		"../Vendor/nlohmann-3.11.3", 
        "../Vendor/Box2D/include",
		"../Vendor/tinyxml2-10.0.0", 
		-- There are a lot of useful sdl structs/classes that the app could use
		"../Vendor/SDL/SDL2-2.30.10/include",
		"../Vendor/SDL/SDL2_image-2.8.2/include",
		"../Vendor/SDL/SDL2_ttf-2.22.0/include",
		"../Vendor/SDL/SDL2_mixer-2.8.0/include"
	}

	links { "2DGameEngine", "Box2D" }

	-- Copy Assets and Shared files in the target dir
	postbuildcommands {
		-- Make sure destination folders exist
		'{MKDIR} "%{cfg.targetdir}/Shared/Resources"',
		'{MKDIR} "%{cfg.targetdir}/App/Assets"',

		-- Use bash to copy contents of the folders, not the folders themselves
		'bash -c "cp -rf ../Shared/Resources/* %{cfg.targetdir}/Shared/Resources/"',
		'bash -c "cp -rf ./Assets/* %{cfg.targetdir}/App/Assets/"'
	}


	-- Linux
	filter "system:linux"
		links { "SDL2", "SDL2_image", "SDL2_ttf", "SDL2_mixer", "Box2D" }

	-- Linux x86
	filter { "system:linux", "platforms:x86" }
		libdirs(table.join(
			get_sdl_libdirs("../Vendor/SDL/", "Linux/", "x86/"),
			get_box2d_libdirs("../Vendor/Box2D/Binaries/")))

	-- Linux x64
	filter { "system:linux", "platforms:x64" }
		libdirs(table.join(
			get_sdl_libdirs("../Vendor/SDL/", "Linux/", "x64/"),
			get_box2d_libdirs("../Vendor/Box2D/Binaries/")))
		
	-- Apparently Windows can resolve links through 2DGameEngine
	

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