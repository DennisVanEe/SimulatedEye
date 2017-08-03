#include "imageplane.hpp"

#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

////////////////////////
// Constants
////////////////////////
const GLfloat EYE_VIEW_QUAD[] =
{
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

ee::ImagePlane::ImagePlane(size_t width, size_t height) :
	m_width(width),
	m_height(height),
	m_imageBuffer(3 * width * height, 0),
	m_texture(0)
{
	GLuint texture_VBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &texture_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, texture_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(EYE_VIEW_QUAD), &EYE_VIEW_QUAD, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_imageBuffer.data());

    glBindTexture(GL_TEXTURE_2D, 0);

	glDeleteBuffers(1, &texture_VBO);
}

ee::ImagePlane::~ImagePlane()
{
	glDeleteTextures(1, &m_texture);
}

bool ee::ImagePlane::setPixel(size_t x, size_t y, Color color)
{
	if (x >= m_width || y >= m_height)
		return false; // this would obviously never work

    x = m_width - x; // flip the image horizontally!

	size_t index = (y * m_width + x) * 3;

	m_imageBuffer[index] = color.r;
	m_imageBuffer[index + 1] = color.g;
	m_imageBuffer[index + 2] = color.b;

	return true;
}

void ee::ImagePlane::calculateImage()
{
	/*glDeleteTextures(1, &m_texture);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_imageBuffer.data());

	glBindTexture(GL_TEXTURE_2D, 0);*/

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_imageBuffer.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ee::ImagePlane::draw(Renderer* renderer)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture); // don't need to send texture, defualts to it

	renderer->getPlaneShader()->use();

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}