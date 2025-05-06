-- Root Build Script (build.lua)
workspace "Project"
	platforms  { "x86", "x64" }
	configurations { "Debug", "Release" }
	startproject "App"  -- Set the app project as the start project

	-- Workspace-wide build options for MSVC (Windows-specific)
	filter "system:windows"
		buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

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

	function get_box2d_libdirs(basePath)
		return {
			basePath .. OutputDir
		}
	end
	
	-- Include Core build script
	include "Core/2DGameEngine/Build-2DGameEngine.lua"
	include "Shared/Build-Shared.lua" 
	include "App/Build-App.lua"
	include "Vendor/Build-Box2D.lua"