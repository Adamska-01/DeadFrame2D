project "2DGameEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files { "include/**.h", "src/**.cpp" }

	local sdl_libs = 
	{
        "SDL2.lib",
        "SDL2main.lib",
        "SDL2_Image.lib",
        "SDL2_ttf.lib",
        "SDL2_mixer.lib"
    }

    -- Include paths
    includedirs 
    { 
        "include",
        "../SDL/SDL2-2.30.10/include", 
        "../SDL/SDL2_image-2.8.2/include", 
        "../SDL/SDL2_ttf-2.22.0/include", 
        "../SDL/SDL2_mixer-2.8.0/include" 
    }
    
     -- Link libraries and set libdirs based on architecture
    filter "platforms:x86"
		links(sdl_libs)
        libdirs 
        { 
            "../SDL/SDL2-2.30.10/lib/x86", 
            "../SDL/SDL2_image-2.8.2/lib/x86", 
            "../SDL/SDL2_ttf-2.22.0/lib/x86", 
            "../SDL/SDL2_mixer-2.8.0/lib/x86" 
        }
        postbuildcommands
        {
            "{MKDIR} ../../Binaries/" .. OutputDir .. "/App/",
            "{COPY} ../SDL/SDL2-2.30.10/lib/x86/*.dll ../../Binaries/" .. OutputDir .. "/App/",
            "{COPY} ../SDL/SDL2_image-2.8.2/lib/x86/*.dll ../../Binaries/" .. OutputDir .. "/App/",
            "{COPY} ../SDL/SDL2_ttf-2.22.0/lib/x86/*.dll ../../Binaries/" .. OutputDir .. "/App/",
            "{COPY} ../SDL/SDL2_mixer-2.8.0/lib/x86/*.dll ../../Binaries/" .. OutputDir .. "/App/"
        }

    filter "platforms:x64"
		links(sdl_libs)
        libdirs 
        { 
            "../SDL/SDL2-2.30.10/lib/x64", 
            "../SDL/SDL2_image-2.8.2/lib/x64", 
            "../SDL/SDL2_ttf-2.22.0/lib/x64", 
            "../SDL/SDL2_mixer-2.8.0/lib/x64" 
        }
        postbuildcommands
        {
			"{MKDIR} ../../Binaries/" .. OutputDir .. "/App/",
			"{COPY} ../SDL/SDL2-2.30.10/lib/x64/*.dll ../../Binaries/" .. OutputDir .. "/App/",
			"{COPY} ../SDL/SDL2_image-2.8.2/lib/x64/*.dll ../../Binaries/" .. OutputDir .. "/App/",
			"{COPY} ../SDL/SDL2_ttf-2.22.0/lib/x64/*.dll ../../Binaries/" .. OutputDir .. "/App/",
			"{COPY} ../SDL/SDL2_mixer-2.8.0/lib/x64/*.dll ../../Binaries/" .. OutputDir .. "/App/"
        }

    -- Reset filter to avoid affecting future configurations
    filter {}
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