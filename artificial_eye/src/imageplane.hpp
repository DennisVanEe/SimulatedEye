#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>

#include "shader.hpp"
#include "drawable.hpp"
#include "renderer.hpp"
#include "type.hpp"

namespace ee
{
	class ImagePlane : public Drawable
	{
	public:
		ImagePlane(size_t width, size_t height);
		~ImagePlane();

		bool setPixel(size_t x, size_t y, Color color); // change an individual pixel color

		void calculateImage();
		size_t getWidth() const
		{
			return m_width;
		}
		size_t getHeight() const
		{
			return m_height;
		}

		void draw(Renderer* renderer) override;

	private:
		// renderer properties:
		GLuint m_texture;
		GLuint m_VAO;

		// image properties:
		std::vector<unsigned char> m_imageBuffer;
		size_t m_width, m_height;
	};
}