#include "textloading.hpp"

GLuint ee::TextureFromFile(std::string name, std::string path)
{
	int width, height, nChannels;
	std::string result = path + name;
	unsigned char* data = stbi_load(result.c_str(), &width, &height, &nChannels, 0);

	if (data == nullptr)
		return 0;

	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	return texture;
}