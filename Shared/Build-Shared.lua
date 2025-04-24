project "Shared"
	kind "Utility"
	language "C++" -- Doesn't really matter
	targetdir ("./Binaries/" .. OutputDir)
	objdir ("./Binaries/Intermediates/" .. OutputDir)
	debugdir "../"

	files {
		"./Configurations/**.*"
	}