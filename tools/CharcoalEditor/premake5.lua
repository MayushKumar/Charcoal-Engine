project "CharcoalEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	IncludeDir["ImGui"] = "Charcoal/vendor/ImGui"
	IncludeDir["GLM"] = "Charcoal/vendor/GLM"
	IncludeDir["stb_image"] = "Charcoal/vendor/stb_image"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"../../Charcoal/src",
		"../../Charcoal/vendor/spdlog/include",
		"../../" .. "%{IncludeDir.GLM}",
		"../../" .. "%{IncludeDir.ImGui}"
	}

	links
	{
	   "Charcoal",
	   "GLFW",
	   "Glad",
	   "ImGui"   
	}

	filter "options:toolset=clang"
		toolset "clang"
		links
		{
		   "shell32.lib",
		   "gdi32.lib"
		}
		linkoptions
		{
		   "-v"
		}
	
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"CH_PLATFORM_WINDOWS"
		}

	filter "system:linux"
		toolset "gcc"
		links
		{
			"X11",
			"dl",
			"pthread",
		}

		defines
		{
			"CH_PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		defines "CH_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CH_RELEASE"
		runtime "Release"
		optimize "On"
