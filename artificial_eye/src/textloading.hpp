#pragma once

#include "stb_image.hpp"
#include <glad/glad.h>
#include <string>

namespace ee
{
	GLuint TextureFromFile(std::string name, std::string path);
}