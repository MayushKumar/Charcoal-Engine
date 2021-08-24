#include <string.h>
#include <iostream>
#include "tiny_gltf.h"

#include "defines.h"

void LoadASCIIGLTF(std::string& filepath)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;
	bool ret = loader.LoadASCIIFromFile(&model, &err,
										&warn, filepath);

	if(!err.empty())
	{
		std::cout << "Error: " << err << "\n";
	}

	if(!warn.empty())
	{
		std::cout << "Warn: " << warn << "\n";
	}

	if(!ret)
	{
		std::cout << "Failed to parse gltf" << "\n";
	}

	CMFFormat format;
	
}
