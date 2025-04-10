project "2DGameEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files { "include/**.h", "src/**.cpp" }

    -- Include paths
    includedirs 
    { 
        "include",
        "../Dependencies/nlohmann-3.11.3", 
        "../Dependencies/tinyxml2-10.0.0", 
        "../Dependencies/SDL/SDL2-2.30.10/include", 
        "../Dependencies/SDL/SDL2_image-2.8.2/include", 
        "../Dependencies/SDL/SDL2_ttf-2.22.0/include", 
        "../Dependencies/SDL/SDL2_mixer-2.8.0/include" 
    }
    
    local sdl_libs = 
	{
        "SDL2.lib",
        "SDL2main.lib",
        "SDL2_Image.lib",
        "SDL2_ttf.lib",
        "SDL2_mixer.lib"
    }

    local copyDir = path.join("../../App/Binaries", OutputDir)

     -- Link libraries and set libdirs based on architecture
    filter "platforms:x86"
		links(sdl_libs)
        libdirs 
        { 
            "../Dependencies/SDL/SDL2-2.30.10/lib/x86", 
            "../Dependencies/SDL/SDL2_image-2.8.2/lib/x86", 
            "../Dependencies/SDL/SDL2_ttf-2.22.0/lib/x86", 
            "../Dependencies/SDL/SDL2_mixer-2.8.0/lib/x86" 
        }
        postbuildcommands
        {
            "{MKDIR} " .. copyDir,
            "{COPY} ../Dependencies/SDL/SDL2-2.30.10/lib/x86/*.dll " .. copyDir,
            "{COPY} ../Dependencies/SDL/SDL2_image-2.8.2/lib/x86/*.dll " .. copyDir,
            "{COPY} ../Dependencies/SDL/SDL2_ttf-2.22.0/lib/x86/*.dll " .. copyDir,
            "{COPY} ../Dependencies/SDL/SDL2_mixer-2.8.0/lib/x86/*.dll " .. copyDir
        }

    filter "platforms:x64"
		links(sdl_libs)
        libdirs 
        { 
            "../Dependencies/SDL/SDL2-2.30.10/lib/x64", 
            "../Dependencies/SDL/SDL2_image-2.8.2/lib/x64", 
            "../Dependencies/SDL/SDL2_ttf-2.22.0/lib/x64", 
            "../Dependencies/SDL/SDL2_mixer-2.8.0/lib/x64" 
        }
        postbuildcommands
        {
            "{MKDIR} " .. copyDir,
			"{COPY} ../Dependencies/SDL/SDL2-2.30.10/lib/x64/*.dll " .. copyDir,
			"{COPY} ../Dependencies/SDL/SDL2_image-2.8.2/lib/x64/*.dll " .. copyDir,
			"{COPY} ../Dependencies/SDL/SDL2_ttf-2.22.0/lib/x64/*.dll " .. copyDir,
			"{COPY} ../Dependencies/SDL/SDL2_mixer-2.8.0/lib/x64/*.dll " .. copyDir
        }

    -- Reset filter to avoid affecting future configurations
    filter {}
    targetdir ("./Binaries/" .. OutputDir)
    objdir ("./Binaries/Intermediates/" .. OutputDir)

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