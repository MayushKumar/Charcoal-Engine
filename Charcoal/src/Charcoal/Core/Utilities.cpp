#include "chpch.h"
#include "Utilities.h"

#include "stb_image.h"
#include "stb_image_write.h"
#include <fstream>

namespace Charcoal
{

	std::vector<char> FileUtils::ReadFileBinary(const std::string &path)
	{
		std::vector<char> result;
		std::ifstream filestream(path, std::ios::binary);
		if (!filestream)
		{
			CH_CORE_ERROR("Failed to open file: {0}", path);
			return result;
		}
		filestream.seekg(0, std::ios::end);
		result.resize(filestream.tellg());
		filestream.seekg(0, std::ios::beg);
		filestream.read(result.data(), result.size());
		filestream.close();
		return result;
	}
		
	ImageUtils::STB_Image ImageUtils::LoadImage(const char* path, bool flipVertically, uint32_t requiredChannels)
	{
		ImageUtils::STB_Image image;
		stbi_set_flip_vertically_on_load(flipVertically);
		{
			CH_PROFILE_SCOPE("Utilities : stbi_image loading");
			int width = 0, height = 0, channels = 0;
			if(stbi_is_hdr(path))
				image.Data = (void*)stbi_loadf(path, &width, &height, &channels, requiredChannels);
			else
				image.Data = stbi_load(path, &width, &height, &channels, requiredChannels);
			image.Width = width;
			image.Height = height;
			image.Channels = channels;
		}
		CH_CORE_ASSERT(image.Data, "Failed to load image!");

		return image;
	}

	void ImageUtils::WritePNGImage(const char* path, bool flipVertically, STB_Image image)
	{
		stbi_flip_vertically_on_write(flipVertically);
		int success = stbi_write_png(path, image.Width, image.Height, image.Channels, image.Data, 0);
		if(!success)
			CH_CORE_ERROR("Failed to write image: {0}", path);
	}

	void ImageUtils::FreeImageData(ImageUtils::STB_Image& image)
	{
		stbi_image_free(image.Data);
	}
	
}
