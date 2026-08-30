-- Root Build Script (build.lua)
workspace "Project"
	platforms  { "x86", "x64" }
	configurations { "Debug", "Release" }
	startproject "App"  -- Set the app project as the start project

	-- Workspace-wide build options for MSVC (Windows-specific)
	filter "system:windows"
		buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus", "/permissive-" }

	-- Output directory structure for builds
	OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"


	-- Helpers
	function get_sdl_libdirs(basePath, platformPath, architecturePath)
		local sdl_version = "2.30.10"
		local image_version = "2.8.2"
		local ttf_version = "2.22.0"
		local mixer_version = "2.8.0"

		return {
			basePath .. "SDL2-" .. sdl_version .. "/lib/" .. platformPath .. "/" .. architecturePath,
			basePath .. "SDL2_image-" .. image_version .. "/lib/" .. platformPath .. "/" .. architecturePath,
			basePath .. "SDL2_ttf-" .. ttf_version .. "/lib/" .. platformPath .. "/" .. architecturePath,
			basePath .. "SDL2_mixer-" .. mixer_version .. "/lib/" .. platformPath .. "/" .. architecturePath,
		}
	end

	function get_libdir(basePath)
		return {
			basePath .. OutputDir
		}
	end

	function make_dir(path)
		if os.host() == "windows" then
			path = path:gsub("/", "\\")
			return '{MKDIR} "' .. path .. '"'
		else
			return 'mkdir -p ' .. path
		end
	end

	function copy_dir(src, dst)
		if os.host() == "windows" then
			src = src:gsub("/", "\\")
			dst = dst:gsub("/", "\\")
			return string.format('xcopy /E /Y /I "%s" "%s"', src, dst)
		else
			return string.format('cp -r %s %s', src, dst)
		end
	end

	function copy_file(src, dst)
		if os.host() == "windows" then
			src = src:gsub("/", "\\")
			dst = dst:gsub("/", "\\")
			return '{COPY} "' .. src .. '" "' .. dst .. '"'
		else
			return 'cp -v ' .. src .. ' ' .. dst
		end
	end

	
	-- Include Core build scripts
	include "DeadFrame2D/Build-DeadFrame2D.lua"
	include "Vendor/Box2D/Build-Box2D.lua"
	include "Vendor/FreeType/Build-FreeType.lua"
	include "Vendor/RmlUi/Build-RmlUi.lua"
	include "Vendor/tinyxml2-10.0.0/Build-tinyxml2.lua"
	include "Tests/Build-Tests.lua"