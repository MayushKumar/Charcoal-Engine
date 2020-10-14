workspace "Charcoal Engine"
	architecture "x64"
	configurations{ "Debug", "Release" }
	startproject "Sandbox"
	
	flags
	{
		"MultiProcessorCompile"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLFW"] = "Charcoal/vendor/GLFW/include"
	IncludeDir["Glad"] = "Charcoal/vendor/Glad/include"
	IncludeDir["ImGui"] = "Charcoal/vendor/ImGui"
	IncludeDir["GLM"] = "Charcoal/vendor/GLM"
	IncludeDir["stb_image"] = "Charcoal/vendor/stb_image"

	group "Dependencies"
		include "Charcoal/vendor/GLFW"
		include "Charcoal/vendor/Glad"
		include "Charcoal/vendor/ImGui"
	group ""

	group "Tools"
		include "tools/CMFCompiler"
		include "tools/CharcoalEditor"
	group ""

	newoption
	{
	   trigger =  "toolset",
	   value = "Compiler",
	   description = "Choose toolset for compiling on windows",
	   allowed =
		  {
			 { "clang", "Clang" },
			 { "msvc",  "Visual Studio" }
		  }
	}
	
	project "Charcoal"
		location "Charcoal"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"%{prj.name}/src/Charcoal/**.h",
			"%{prj.name}/src/Charcoal/**.cpp",
			"%{prj.name}/src/Charcoal.h",
			"%{prj.name}/src/chpch.h",
			"%{prj.name}/src/chpch.cpp",
			"%{prj.name}/src/Platform/OpenGL/**.h",
			"%{prj.name}/src/Platform/OpenGL/**.cpp",
			"%{prj.name}/vendor/stb_image/stb_image.h",
			"%{prj.name}/vendor/stb_image/stb_image.cpp"
		}

		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.GLM}",
			"%{IncludeDir.stb_image}",
			"%{IncludeDir.tinyobjloader}",
			"%{IncludeDir.tinygltf}"
		}

		links
		{
			"GLFW",
			"Glad",
			"ImGui"
		}

		defines
		{
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

		pchheader "chpch.h"
		pchsource "%{prj.name}/src/chpch.cpp"

		filter "options:toolset=clang"
			toolset "clang"
			buildoptions
			{
			   "-Wno-deprecated-declarations"
			}
		
		filter "system:windows"
			systemversion "latest"
			defines
			{
				"CH_PLATFORM_WINDOWS"
			}
			files
			{
				"%{prj.name}/src/Platform/Windows/**.h",
				"%{prj.name}/src/Platform/Windows/**.cpp",
			}

		filter "system:linux"
			toolset "gcc"
			defines
			{
				"CH_PLATFORM_LINUX"
			}
			files
			{
				"%{prj.name}/src/Platform/Linux/**.h",
				"%{prj.name}/src/Platform/Linux/**.cpp",
			}

		filter "configurations:Debug"
			runtime "Debug"
			symbols "On"
			defines
			{
				"CH_ENABLE_ASSERTS",
				"CH_DEBUG"
			}

		filter "configurations:Release"
			defines "CH_RELEASE"
			runtime "Release"
			optimize "On"

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
		
		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"Charcoal/src",
			"Charcoal/vendor/spdlog/include",
			"%{IncludeDir.GLM}",
			"%{IncludeDir.ImGui}"
		}

		links
		{
		   "Charcoal",
		   "GLFW",
		   "Glad",
		   "ImGui"
		}

		filter "options:toolset=clang"
			toolset "gcc"
--			links
--			{
--			   "shell32.lib",
--			   "gdi32.lib"
--			}
--			linkoptions
--			{
--			   "-v"
--			}
		
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
