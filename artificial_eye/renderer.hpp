#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shader.hpp"
#include "camera.hpp"
#include "drawable.hpp"

namespace syn_eye
{
	class Renderer
	{
	public:
		static Renderer& startRenderer(size_t width, size_t height);
		~Renderer();
		
		// for rendering:
		bool keepRendering() const; // true if the loop should keep going, false otherwise
		void processInput(float deltaTime);
		void mouseCallback(GLFWwindow* window, double xpos, double ypos);
		void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		void draw(Drawable& drawable);

		GLFWwindow* getWindowPtr()
		{
			return m_window;
		}

		Shader* get3Dshader() { return &m_3DShader; }
		Shader* getPlaneShader() { return &m_planeShader;  }

		glm::mat4 getViewMat() const { return m_camera.viewMatrix(); }
		glm::mat4 getProjMat() const { return m_perspectiveMat; }

	private:
		Renderer(size_t width, size_t height);

		GLFWwindow* m_window;
		Camera m_camera;
		glm::mat4 m_perspectiveMat;

		Shader m_3DShader, m_planeShader;

		// mouse
		bool m_firstMouse;
		float m_lastX;
		float m_lastY;

	public:
		Renderer(const Renderer&) = delete;
		Renderer(const Renderer&&) = delete;
		void operator=(const Renderer&) = delete;
	};
}