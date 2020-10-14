#pragma once

#include "string.h"
#include "stb_image.h"

namespace Charcoal
{

	class FileUtils
	{
	public:
		static std::vector<char> ReadFileBinary(const std::string& path);
	};

	class ImageUtils
	{
	public:
		
		struct STB_Image
		{
			uint32_t Width, Height;
			uint32_t Channels;
			stbi_uc* Data;
		};
		
		static STB_Image LoadImage(const std::string& path, bool flipVertical, uint32_t requiredChannels = 0);
		
	};
	
}
