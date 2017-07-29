#include "renderer.hpp"

#include <iostream>
#include <string>
#include <exception>

using namespace std;

syn_eye::Renderer& syn_eye::Renderer::startRenderer(size_t width, size_t height)
{
	static Renderer inst(width, height);
	return inst;
}

syn_eye::Renderer::~Renderer()
{
	glfwTerminate();
}

syn_eye::Renderer::Renderer(size_t width, size_t height) :
	m_camera(glm::vec3(10.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 180, 0),
	m_firstMouse(true)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, "SyntheticEye", NULL, NULL);
	assert(m_window);

	glfwMakeContextCurrent(m_window);

	assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

	m_perspectiveMat = glm::perspective(glm::radians(45.f), (float)width / (float)height, 0.1f, 100.0f);
	
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	m_3DShader.initialize("shader/3D_vert.glsl", "shader/3D_frag.glsl");
	m_planeShader.initialize("shader/eyeview_vert.glsl", "shader/eyeview_frag.glsl");
}

bool syn_eye::Renderer::keepRendering() const
{
	return !glfwWindowShouldClose(m_window);
}

void syn_eye::Renderer::processInput(float deltaTime)
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	// position:
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
		m_camera.processKBInput(FORWARD, deltaTime);
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
		m_camera.processKBInput(BACKWARD, deltaTime);
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
		m_camera.processKBInput(LEFT, deltaTime);
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
		m_camera.processKBInput(RIGHT, deltaTime);
}

void syn_eye::Renderer::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (m_firstMouse)
	{
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstMouse = false;
	}

	float xoffset = xpos - m_lastX;
	float yoffset = m_lastY - ypos; // reversed since y-coordinates go from bottom to top

	m_lastX = xpos;
	m_lastY = ypos;

	m_camera.processMInput(xoffset, yoffset);
}

void syn_eye::Renderer::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_camera.processSInput(yoffset);
}

void syn_eye::Renderer::draw(Drawable& item)
{
	Renderer* render = this;
	item.draw(render);
}