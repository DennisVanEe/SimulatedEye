#pragma once

#include "stb_image.hpp"
#include <glad/glad.h>
#include <string>

namespace syn_eye
{
	GLuint TextureFromFile(std::string name, std::string path);
}