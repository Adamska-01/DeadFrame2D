project "Tests"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)
	debugdir "../"

	defines { "DF2D_DYNAMIC" }

	dependson { "DeadFrame2D", "Box2D", "tinyxml2" }

	files { 
		"**.cpp",
	}

	includedirs { 
		"include",
		"../DeadFrame2D/include",
		"../DeadFrame2D/Internal",
		"../Vendor/nlohmann-3.11.3", 
		"../Vendor/tinyxml2-10.0.0", 
		"../Vendor/Box2D/Module/include",
		"../Vendor/SDL/SDL2-2.30.10/include", 
		"../Vendor/SDL/SDL2_image-2.8.2/include", 
		"../Vendor/SDL/SDL2_mixer-2.8.0/include",
		"../Vendor/Doctest/Module/doctest"
	}

	links { "DeadFrame2D", "SDL2", "SDL2_image", "SDL2_mixer", "Box2D", "tinyxml2" }


	function configure_platform_libs(osName, arch)
		libdirs(table.join(
			get_sdl_libdirs("../Vendor/SDL/", osName, arch),
			get_libdir("../DeadFrame2D/Binaries/"),
			get_libdir("../Vendor/Box2D/Binaries/"),
			get_libdir("../Vendor/tinyxml2-10.0.0/Binaries/")
		))
	end


	function configure_postbuild(osName, resolvedArch)
		local libs = {
			SDL2       = "2.30.10",
			SDL2_image = "2.8.2",
			SDL2_mixer = "2.8.0"
		}
		local engineDir = "../DeadFrame2D/Binaries/" .. osName .. "-" .. "%{cfg.architecture}" .. "/%{cfg.buildcfg}/"
		local sdlDir = "../Vendor/SDL/"

		local commands = {}
		table.insert(commands, make_dir("%{cfg.targetdir}"))
		table.insert(commands, copy_file(engineDir .. (osName == "windows" and "DeadFrame2D.dll" or "libDeadFrame2D.so"), "%{cfg.targetdir}"))

		for lib, version in pairs(libs) do
			local ext = osName == "windows" and "dll" or "so.0"
			local platformDir = osName == "windows" and "Windows" or "Linux"

			table.insert(
				commands,
				copy_file(
					sdlDir .. lib .. "-" .. version .. "/lib/" .. platformDir .. "/" .. resolvedArch .. "/*." .. ext,
					"%{cfg.targetdir}"))
		end

		postbuildcommands(commands)
	end


	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x64"


	filter "system:windows"
		implibdir ("./Binaries/" .. OutputDir)

	filter { "system:windows", "platforms:x86" }
		configure_platform_libs("Windows", "x86")
		configure_postbuild("windows", "x86")
	
	filter { "system:windows", "platforms:x64" }
		configure_platform_libs("Windows", "x64")
		configure_postbuild("windows", "x64")


	filter { "system:linux", "platforms:x86" }
		configure_platform_libs("Linux", "x86")
		configure_postbuild("linux", "x86")
	
	filter { "system:linux", "platforms:x64" }
		configure_platform_libs("Linux", "x64")
		configure_postbuild("linux", "x64")


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