project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		--buildoptions { "-std=c11", "-lgdi32" }
		systemversion "latest"

	filter "configurations:Debug"
			runtime "Debug"
			symbols "On"

	filter "configurations:Release"
			runtime "Release"
			optimize "On"