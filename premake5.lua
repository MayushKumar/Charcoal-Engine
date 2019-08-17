workspace "Charcoal Engine"
	architecture "x64"
	configurations{ "Debug", "Release" }
	startproject "Sandbox"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	project "Charcoal"
		location "Charcoal"
		kind "StaticLib"
		language "C++"
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
			"vendor/spdlog/include"
		}

		pchheader "chpch.h"
		pchsource "%{prj.name}/src/chpch.cpp"

		filter "system:windows"
			cppdialect "C++17"
			systemversion "latest"
			defines
			{
				"CH_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "CH_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "CH_RELEASE"
			optimize "On"

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"
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
			"vendor/spdlog/include"
		}

		links
		{
			"Charcoal"
		}

		filter "system:windows"
			cppdialect "C++17"
			systemversion "latest"
			defines
			{
				"CH_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "CH_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "CH_RELEASE"
			optimize "On"