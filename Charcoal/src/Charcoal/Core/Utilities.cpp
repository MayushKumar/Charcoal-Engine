#include "chpch.h"
#include "Utilities.h"

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
		
	ImageUtils::STB_Image ImageUtils::LoadImage(const std::string& path, bool flipVertical, uint32_t requiredChannels)
	{
		ImageUtils::STB_Image image;
		stbi_set_flip_vertically_on_load(flipVertical);
		{
			CH_PROFILE_SCOPE("Utilities : stbi_image loading");
			int width = 0, height = 0, channels = 0;
			image.Data = stbi_load(path.c_str(), &width, &height, &channels, requiredChannels);
			image.Width = width;
			image.Height = height;
			image.Channels = channels;
		}
		CH_CORE_ASSERT(image.Data, "Failed to load image!");

		return image;
	}
	
}
