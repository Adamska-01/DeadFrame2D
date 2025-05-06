project "2DGameEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)
	dependson { "Box2D" }
	staticruntime "on"
	debugdir "../../" -- Set working directory to the root of the solution


	files { 
		"include/**.h", 
		"src/**.cpp",
		"../../Vendor/tinyxml2-10.0.0/**.cpp" 
	}

	includedirs { 
		"include",
		"../../Shared/include", 
		"../../Vendor/nlohmann-3.11.3", 
		"../../Vendor/tinyxml2-10.0.0", 
		"../../Vendor/Box2D/include",
		"../../Vendor/SDL/SDL2-2.30.10/include", 
		"../../Vendor/SDL/SDL2_image-2.8.2/include", 
		"../../Vendor/SDL/SDL2_ttf-2.22.0/include", 
		"../../Vendor/SDL/SDL2_mixer-2.8.0/include" 
	}

	local copyDir = path.join("../../App/Binaries", OutputDir)

	-- Linux
	filter "system:linux"
		links { "SDL2", "SDL2_image", "SDL2_ttf", "SDL2_mixer", "Box2D" }

	-- Linux x86
	filter { "system:linux", "platforms:x86" }
		architecture "x86"
		libdirs(table.join(
			get_sdl_libdirs("../../Vendor/SDL/", "Linux/", "x86/"),
			get_box2d_libdirs("../../Vendor/Box2D/Binaries/")))
		postbuildcommands {
			"mkdir -p " .. copyDir,
			"cp -v ../../Vendor/SDL/SDL2-2.30.10/lib/Linux/x86/*.so " .. copyDir,
			"cp -v ../../Vendor/SDL/SDL2_image-2.8.2/lib/Linux/x86/*.so " .. copyDir,
			"cp -v ../../Vendor/SDL/SDL2_ttf-2.22.0/lib/Linux/x86/*.so " .. copyDir,
			"cp -v ../../Vendor/SDL/SDL2_mixer-2.8.0/lib/Linux/x86/*.so " .. copyDir
		}

	-- Linux x64
	filter { "system:linux", "platforms:x64" }
		architecture "x86_64"
		libdirs(table.join(
			get_sdl_libdirs("../../Vendor/SDL/", "Linux/", "x64/"),
			get_box2d_libdirs("../../Vendor/Box2D/Binaries/")))
		postbuildcommands {
			"mkdir -p " .. copyDir,
			"cp -v ../../Vendor/SDL/SDL2-2.30.10/lib/Linux/x64/*.so " .. copyDir,
			"cp -v ../../Vendor/SDL/SDL2_image-2.8.2/lib/Linux/x64/*.so " .. copyDir,
			"cp -v ../../Vendor/SDL/SDL2_ttf-2.22.0/lib/Linux/x64/*.so " .. copyDir,
			"cp -v ../../Vendor/SDL/SDL2_mixer-2.8.0/lib/Linux/x64/*.so " .. copyDir
		}

	-- Windows
	filter "system:windows"
		links { "SDL2.lib", "SDL2main.lib", "SDL2_Image.lib", "SDL2_ttf.lib", "SDL2_mixer.lib", "Box2D.lib" }
		
	-- Windows x86
	filter { "system:windows", "platforms:x86" }
		architecture "x86"
		libdirs(table.join(
			get_sdl_libdirs("../../Vendor/SDL/", "Windows/", "x86/"),
			get_box2d_libdirs("../../Vendor/Box2D/Binaries/")))
		postbuildcommands{
			"{MKDIR} " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2-2.30.10/lib/Windows/x86/*.dll " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2_image-2.8.2/lib/Windows/x86/*.dll " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2_ttf-2.22.0/lib/Windows/x86/*.dll " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2_mixer-2.8.0/lib/Windows/x86/*.dll " .. copyDir
		}

	-- Windows x64
	filter { "system:windows", "platforms:x64" }
		architecture "x64"
		libdirs(table.join(
			get_sdl_libdirs("../../Vendor/SDL/", "Windows/", "x64/"),
			get_box2d_libdirs("../../Vendor/Box2D/Binaries/")))
		postbuildcommands {
			"{MKDIR} " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2-2.30.10/lib/Windows/x64/*.dll " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2_image-2.8.2/lib/Windows/x64/*.dll " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2_ttf-2.22.0/lib/Windows/x64/*.dll " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2_mixer-2.8.0/lib/Windows/x64/*.dll " .. copyDir
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