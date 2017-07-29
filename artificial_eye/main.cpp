#include <iostream>
#include <cassert>

#include "eyeball.hpp"
#include "renderer.hpp"
#include "world.hpp"

#include <glm/gtx/transform.hpp>

using namespace glm;
using namespace std;
using namespace syn_eye;

enum class RenderState {WORLD_VIEW, EYE_VIEW};

// not the cleanest or most efficient code, but it works, and that is good enough for me.

Renderer* g_renderer;
EyeBall* g_eyeball;
RenderState g_currRenderState = RenderState::WORLD_VIEW;
bool g_updateEyeprop = g_currRenderState == RenderState::EYE_VIEW ? true : false;

// need to do this or else we can't add callback support.
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	assert(g_renderer);
	g_renderer->mouseCallback(window, xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	assert(g_renderer);
	g_renderer->scrollCallback(window, xoffset, yoffset);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// switch the render state when the appropriate key is pressed
	if (key == GLFW_KEY_F2)
	{
		if (action == GLFW_PRESS)
		{
			if (g_currRenderState == RenderState::EYE_VIEW)
				g_currRenderState = RenderState::WORLD_VIEW;
			else
			{
				g_currRenderState = RenderState::EYE_VIEW;
				g_updateEyeprop = true;
			}
		}
	}

    glm::mat4 trans;
    switch (key)
    {
    case GLFW_KEY_UP:
        trans = g_eyeball->getTransform();
        trans = trans * glm::rotate(glm::radians(-0.5f), glm::vec3(1.f, 0.f, 0.f));
        g_eyeball->transform(trans);
        g_updateEyeprop = true;
        break;
    case GLFW_KEY_DOWN:
        trans = g_eyeball->getTransform();
        trans = trans * glm::rotate(glm::radians(0.5f), glm::vec3(1.f, 0.f, 0.f));
        g_eyeball->transform(trans);
        g_updateEyeprop = true;
        break;
    case GLFW_KEY_LEFT:
        trans = g_eyeball->getTransform();
        trans = trans * glm::rotate(glm::radians(0.5f), glm::vec3(0.f, 1.f, 0.f));
        g_eyeball->transform(trans);
        g_updateEyeprop = true;
        break;
    case GLFW_KEY_RIGHT:
        trans = g_eyeball->getTransform();
        trans = trans * glm::rotate(glm::radians(-0.5f), glm::vec3(0.f, 1.f, 0.f));
        g_eyeball->transform(trans);
        g_updateEyeprop = true;
        break;
    }
}

int main()
{
	bool correctInput = false;
    size_t screenWidth = 1280;
    size_t screenHeight = 720;

	g_renderer = &(Renderer::startRenderer(screenWidth, screenHeight));

	// initialize the world (might later use a file to add this)
	World world;
	world.addSphere(RaySphere(vec3(0.f, 0.f, 10.f), vec3(1.f, 1.f, 1.f)));

	// initialize the image buffer the eye will paint to
	ImagePlane imageBuffer(screenWidth, screenHeight);
    g_eyeball = new EyeBall(vec3(0.f, 0.f, 0.f), 1.f, 0.5f, 0.281f, &imageBuffer, &world);

	// set up the renderer's callbacks:
	glfwSetCursorPosCallback(g_renderer->getWindowPtr(), mouseCallback);
	glfwSetScrollCallback(g_renderer->getWindowPtr(), scrollCallback);
	glfwSetKeyCallback(g_renderer->getWindowPtr(), keyboardCallback);

	float deltaTime, lastFrame;
	deltaTime = lastFrame = 0;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (g_renderer->keepRendering())
	{
		// calculate the time elapsed:
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		g_renderer->processInput(deltaTime);

		// do the actual rendering:
		switch (g_currRenderState)
		{
		case RenderState::EYE_VIEW:
			glClearColor(0.f, 0.f, 0.f, 1.f); // clear to the default color
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			if (g_updateEyeprop)
			{
				glfwSwapBuffers(g_renderer->getWindowPtr());
				g_eyeball->calculateImage();
				imageBuffer.calculateImage();
				g_updateEyeprop = false;
			}
			g_renderer->draw(imageBuffer);
			break;
			
		case RenderState::WORLD_VIEW: // not supper efficient, but it works for now
			glClearColor(0.1f, 0.1f, 0.1f, 1.f); // clear to the default color
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			g_renderer->draw(*g_eyeball);
			vector<RaySphere>& spheres = world.getSpheres();
			for (RaySphere& s : spheres)
				g_renderer->draw(s);
			break;
		}

		glfwSwapBuffers(g_renderer->getWindowPtr());
		glfwPollEvents();
	}
	glfwTerminate();
	delete g_eyeball;

	std::cin.get();
	std::cin.get();
	return 0;
}
