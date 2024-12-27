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

   -- Include Core build script
   include "Core/2DGameEngine/Build-2DGameEngine.lua"
   include "App/Build-App.lua" 