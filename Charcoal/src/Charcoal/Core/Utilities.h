#pragma once

#include "string.h"

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
			void* Data;
		};
		
		static STB_Image LoadImage(const char* path, bool flipVertically, uint32_t requiredChannels = 0);
		static void FreeImageData(STB_Image& image);

		static void WritePNGImage(const char* path, bool flipVertically, STB_Image image);
		
	};
	
}
