project "2DGameEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir ("./Binaries/" .. OutputDir)
    objdir ("./Binaries/Intermediates/" .. OutputDir)
    staticruntime "off"
    debugdir "../../"
    
    files 
    { 
        "include/**.h", 
        "src/**.cpp",
		"../../Vendor/tinyxml2-10.0.0/**.cpp" 
    }

    -- Include paths
    includedirs 
    { 
        "include",
        "../../Vendor/nlohmann-3.11.3", 
        "../../Vendor/tinyxml2-10.0.0", 
        "../../Vendor/SDL/SDL2-2.30.10/include", 
        "../../Vendor/SDL/SDL2_image-2.8.2/include", 
        "../../Vendor/SDL/SDL2_ttf-2.22.0/include", 
        "../../Vendor/SDL/SDL2_mixer-2.8.0/include" 
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
        architecture "x86"
		links(sdl_libs)
        libdirs 
        { 
            "../../Vendor/SDL/SDL2-2.30.10/lib/x86", 
            "../../Vendor/SDL/SDL2_image-2.8.2/lib/x86", 
            "../../Vendor/SDL/SDL2_ttf-2.22.0/lib/x86", 
            "../../Vendor/SDL/SDL2_mixer-2.8.0/lib/x86" 
        }
        postbuildcommands
        {
            "{MKDIR} " .. copyDir,
            "{COPY} ../../Vendor/SDL/SDL2-2.30.10/lib/x86/*.dll " .. copyDir,
            "{COPY} ../../Vendor/SDL/SDL2_image-2.8.2/lib/x86/*.dll " .. copyDir,
            "{COPY} ../../Vendor/SDL/SDL2_ttf-2.22.0/lib/x86/*.dll " .. copyDir,
            "{COPY} ../../Vendor/SDL/SDL2_mixer-2.8.0/lib/x86/*.dll " .. copyDir
        }

    filter "platforms:x64"
        architecture "x64"
		links(sdl_libs)
        libdirs 
        { 
            "../../Vendor/SDL/SDL2-2.30.10/lib/x64", 
            "../../Vendor/SDL/SDL2_image-2.8.2/lib/x64", 
            "../../Vendor/SDL/SDL2_ttf-2.22.0/lib/x64", 
            "../../Vendor/SDL/SDL2_mixer-2.8.0/lib/x64" 
        }
        postbuildcommands
        {
            "{MKDIR} " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2-2.30.10/lib/x64/*.dll " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2_image-2.8.2/lib/x64/*.dll " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2_ttf-2.22.0/lib/x64/*.dll " .. copyDir,
			"{COPY} ../../Vendor/SDL/SDL2_mixer-2.8.0/lib/x64/*.dll " .. copyDir
        }
        
    -- Reset filter (The following applies to every platform)
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