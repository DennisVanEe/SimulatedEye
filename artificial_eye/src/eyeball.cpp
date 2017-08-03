#include "eyeball.hpp"

#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

const std::string EYE_MODEL_DIR = "models/eyeball/eyebal_fbx.fbx";

#include <iostream>

ee::EyeBall::EyeBall(glm::vec3 pos, float radius, float retinaWidth, float retinaHeight, ImagePlane* image, World* world) :
	m_transform(),
	m_world(world),
	m_imageBuffer(image),
	m_model(EYE_MODEL_DIR)
{
	assert(m_model.load());

	radius *= 0.5; // this is just a hack, so, yeah (specific to the model I am using...)

	m_transform = translate(pos);
	m_transform *= scale(vec3(radius, radius, radius)); // set the appropriate transformation

	float maxWidth = pi<float>() * retinaWidth;
	float maxHeight = pi<float>() * retinaHeight;

	m_incWidth = maxWidth / m_imageBuffer->getWidth();
	m_incHeight = maxHeight / m_imageBuffer->getHeight();

	m_startWidth = maxWidth / 2;
	m_startHeight = (pi<float>() / 2) - (maxHeight / 2);
}

void ee::EyeBall::calculateImage()
{
	float width = m_startWidth;
	for (size_t x = 0; x < m_imageBuffer->getWidth(); x++)
	{
		float height = m_startHeight;
		for (size_t y = 0; y < m_imageBuffer->getHeight(); y++)
		{
			vec4 startPoint(sinf(height) * sin(width), cosf(height), -sinf(height) * cos(width), 1.f); // parametric equation of sphere
			vec4 endPoint(0.f, 0.f, 1.f, 1.f); // along the z-axis

			// transform them:
			endPoint = m_transform * endPoint;
			startPoint = m_transform * startPoint;

			// calculate the ray;
			Ray ray;
			ray.dir = vec3(endPoint - startPoint);
			ray.origin = vec3(endPoint);

			// do the ray tracing
			Color color = rayTrace(ray);
		
			// add it to the end result
			if (!m_imageBuffer->setPixel(x, y, color))
			{
				assert(false);
			}

			height += m_incHeight;
		}

		width -= m_incWidth;
	}
}

void ee::EyeBall::draw(Renderer* renderer)
{
	Shader* shader = renderer->get3Dshader();
	
	shader->use();
	shader->assignMat4("model", glm::value_ptr(m_transform));
	glm::mat4 cameraMat = renderer->getViewMat();
	shader->assignMat4("view", glm::value_ptr(cameraMat));
	shader->assignMat4("projection", glm::value_ptr(renderer->getProjMat()));

	m_model.draw(*shader);
}

ee::Color ee::EyeBall::rayTrace(Ray ray)
{
	const vector<RaySphere>& sphereList = m_world->getSpheres();

	for (const RaySphere& s : sphereList)
	{
		if (s.intersect(ray, 0).first > 0)
			return s.getColor();
	}
	return Color(0, 0, 0); // black for now
}