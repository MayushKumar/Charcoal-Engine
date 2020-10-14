project "CMFCompiler"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"../Charcoal/vendor/stb_image/stb_image.h",
		"vendor/tinyobjloader/tiny_obj_loader.h",
		"vendor/tinyobjloader/tiny_obj_loader.cc",
		--"%{prj.name}/vendor/tinygltf/tiny_gltf.h",
		--"%{prj.name}/vendor/tinygltf/tiny_gltf.cpp",
		--"%{prj.name}/vendor/tinygltf/stb_image_write.h" --Dependency for tiny_gltf only
	}

	includedirs
	{
		"src",
		--"%{IncludeDir.GLM}",
		"../../" .. "%{IncludeDir.stb_image}",
		"../../" .. "%{IncludeDir.GLM}",
		"vendor/tinyobjloader",
		--"%{IncludeDir.tinygltf}"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"CH_PLATFORM_WINDOWS"
		}
		
	filter "system:linux"
		toolset "gcc"
		defines
		{
			"CH_PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
