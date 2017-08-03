#include <iostream>
#include <cassert>

#include "eyeball.hpp"
#include "renderer.hpp"
#include "world.hpp"

#include <glm/gtx/transform.hpp>

#include <vector>
#include <cstdlib>
#include <ctime>

using namespace glm;
using namespace std;
using namespace ee;

enum class RenderState {WORLD_VIEW, EYE_VIEW};

struct RaySphereParam
{
	vec3 m_position;
	vec3 m_size;
	Color m_color;
};

/////////////////////////////////////////////////
// USER MOD. SETTINGS
/////////////////////////////////////////////////

const size_t g_screenWidth = 1280; // resolution at which the eye sees and the window is rendered at
const size_t g_screenHeight = 720;

const float g_widthPercent = 0.5f; // this can be used to change the fov. It represents the percent of the semisphere of the eye that takes up the
								   // the retina. For instance, 0.5 means that 45 degrees width of the eye-sphere is the width of the retina in the back (this might be confusing, it's hard to explain...)
const float g_heightPercent = 0.281f; // make sure that g_widthPercent / g_heightPercent == g_screenWidth / g_screenHeight or the image will be stretched.

const vec3 g_randomRange = vec3(1.f, 1.f, 1.f); // range of movememnt for all spheres (e.g. x = rand(-g_randomRange.x, g_randomRange.x), etc.)

const RaySphereParam g_raySpheres[] = // list of spheres in the world, all subject to move randomly when space is pressed
{
	// Position, size (lengths along axis), color
	{ vec3(0.f, 0.f, 10.f), vec3(1.f, 1.f, 1.f), Color(255, 21, 255) },
	{ vec3(0.f, 10.f, 10.f), vec3(1.f, 0.5f, 1.f), Color(255, 21, 255) }
};

/////////////////////////////////////////////////

Renderer* g_renderer;
EyeBall* g_eyeball;
World g_world;
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

float randf_abs(float min)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = 2 * min;
	float range = random * diff;
	return -min + range;
}

void moveRandomSpheres()
{
	auto& spheres = g_world.getSpheres();
	for (auto& sphere : spheres)
	{
		vec3 offset = vec3(randf_abs(abs(g_randomRange.x)), randf_abs(abs(g_randomRange.y)), randf_abs(abs(g_randomRange.z)));
		glm::mat4 currTrans = sphere.getTransform();
		currTrans = glm::translate(currTrans, offset);
		sphere.setTransform(currTrans);
	}
	g_updateEyeprop = true;
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

	if (key == GLFW_KEY_SPACE)
	{
		if (action == GLFW_PRESS)
		{
			moveRandomSpheres();
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
	std::srand(std::time(nullptr));

	g_renderer = &(Renderer::startRenderer(g_screenWidth, g_screenHeight));

	for (const auto& sphere : g_raySpheres)
	{
		g_world.addSphere(RaySphere(sphere.m_position, sphere.m_size, sphere.m_color));
	}

	// initialize the image buffer the eye will paint to
	ImagePlane imageBuffer(g_screenWidth, g_screenHeight);
	g_eyeball = new EyeBall(vec3(0.f, 0.f, 0.f), 1.f, g_widthPercent, g_heightPercent, &imageBuffer, &g_world);

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
			vector<RaySphere>& spheres = g_world.getSpheres();
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
