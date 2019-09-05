workspace "Charcoal Engine"
	architecture "x64"
	configurations{ "Debug", "Release" }
	startproject "Sandbox"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLFW"] = "Charcoal/vendor/GLFW/include"
	IncludeDir["Glad"] = "Charcoal/vendor/Glad/include"
	IncludeDir["ImGui"] = "Charcoal/vendor/ImGui"

	group "Dependencies"
		include "Charcoal/vendor/GLFW"
		include "Charcoal/vendor/Glad"
		include "Charcoal/vendor/ImGui"
	group ""

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
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}"
		}

		links
		{
			"GLFW",
			"Glad",
			"ImGui"
			--"opengl32.lib"
		}

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

		pchheader "chpch.h"
		pchsource "%{prj.name}/src/chpch.cpp"

		filter "system:windows"
			systemversion "latest"
			defines
			{
				"CH_PLATFORM_WINDOWS"
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
			"Charcoal/vendor/spdlog/include"
		}

		links
		{
			"Charcoal"
		}

		filter "system:windows"
			systemversion "latest"
			defines
			{
				"CH_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "CH_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "CH_RELEASE"
			runtime "Release"
			optimize "On"