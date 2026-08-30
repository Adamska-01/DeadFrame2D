project "DeadFrame2D"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("./Binaries/" .. OutputDir) -- .dll
	objdir ("./Binaries/Intermediates/" .. OutputDir)
	debugdir "../" -- Set working directory to the root of the solution

	defines { "DF2D_DYNAMIC", "DF2D_DYNAMIC_BUILD", "RMLUI_STATIC_LIB" }

	dependson { "Box2D", "FreeType", "RmlUi", "tinyxml2" }

	files { 
		"include/**.h", 
		"src/**.cpp",
		"Internal/include/**.h",
		"Internal/src/**.cpp",
		"Resources/**.*"
	}

	includedirs { 
		"include",
		"Internal",
		"Internal/include",
		"../Vendor/nlohmann-3.11.3", 
		"../Vendor/tinyxml2-10.0.0", 
		"../Vendor/Box2D/Module/include",
		"../Vendor/FreeType/Module/include",
		"../Vendor/RmlUi/Module/Include",
		"../Vendor/SDL/SDL2-2.30.10/include", 
		"../Vendor/SDL/SDL2_image-2.8.2/include", 
		"../Vendor/SDL/SDL2_mixer-2.8.0/include" 
	}

	links { "Box2D", "FreeType", "RmlUi", "tinyxml2", "SDL2", "SDL2_image", "SDL2_mixer" }


	-- Utility to configure platform + arch libdirs in a single call
	function configure_platform_libs(osName, arch)
		libdirs(table.join(
			get_sdl_libdirs("../Vendor/SDL/", osName, arch),
			get_libdir("../Vendor/Box2D/Binaries/"),
			get_libdir("../Vendor/FreeType/Binaries/"),
			get_libdir("../Vendor/RmlUi/Binaries/"),
			get_libdir("../Vendor/tinyxml2-10.0.0/Binaries/")
		))
	end


	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x64"


	-- Windows
	filter "system:windows"
		implibdir ("./Binaries/" .. OutputDir)

	filter { "system:windows", "platforms:x86" }
		configure_platform_libs("Windows", "x86")
	
	filter { "system:windows", "platforms:x64" }
		configure_platform_libs("Windows", "x64")


	-- Linux
	filter { "system:linux", "platforms:x86" }
		configure_platform_libs("Linux", "x86")
	
	filter { "system:linux", "platforms:x64" }
		configure_platform_libs("Linux", "x64")


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