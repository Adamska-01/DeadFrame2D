project "App"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	targetdir "Binaries/%{cfg.buildcfg}"
	staticruntime "off"

	files { "include/**.h", "src/**.cpp" }

	includedirs
	{
		-- Include Core
		"include",
		"../Core/2DGameEngine/include",
		-- There are a lot of useful sdl structs/classes that the app could use
		"../Core/Dependencies/SDL/SDL2-2.30.10/include",
		"../Core/Dependencies/SDL/SDL2_image-2.8.2/include",
		"../Core/Dependencies/SDL/SDL2_ttf-2.22.0/include",
		"../Core/Dependencies/SDL/SDL2_mixer-2.8.0/include"
	}

	links
	{
		"../Core/Binaries/" .. OutputDir .. "/2DGameEngine/2DGameEngine.lib"
	}

	-- Ensure the app depends on the 2DGameEngine project
	dependson { "2DGameEngine" }
	
	targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
	objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

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