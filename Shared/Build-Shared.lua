project "Shared"
	kind "StaticLib"
	language "C++" -- Doesn't really matter
	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)
	debugdir "../"

	files { 
		"./include/**.*",
		"./Configurations/**.*",
		"./Resources/**.*"
	}

	includedirs {
		"include",
	}