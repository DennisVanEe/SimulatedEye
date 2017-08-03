#include "raysphere.hpp"
#include "glm/vec4.hpp"
#include "renderer.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;
using namespace std;

const std::string BALL_MODEL_DIR = "models/ball/ball.obj";
ee::Model ee::RaySphere::m_model(BALL_MODEL_DIR); // this is where the static goes

int sign(float val)
{
	return (0 < val) - (val < 0);
}

ee::RaySphere::RaySphere(const glm::vec3 position, const glm::vec3 size, const Color color) :
	m_color(color)
{
	m_model.load();

	// calculate the resulting matrix
	m_trans = translate(position);
	m_trans = m_trans * scale(size);

	// also, calculate the inverse
	m_inverse = inverse(m_trans);
}

ee::RaySphere::RaySphere(const glm::vec3 size, Color color) :
	m_trans(),
	m_color(color)
{
	m_model.load();

	m_trans *= scale(size);

	// if we want to store the inverse as well
	m_inverse = inverse(m_trans);
}

ee::RaySphere::RaySphere() :
	m_trans(), // should default to identity
	m_inverse()
{
	m_model.load();
}

pair<float, glm::vec3> ee::RaySphere::intersect(Ray ray, float min_dist) const
{
	float max_dist = 100000.f;

	vec3 inv_dir = vec3(m_inverse * vec4(ray.dir, 0.f)); // the direction is the one that is provided
	vec3 inv_origin = vec3(m_inverse * vec4(ray.origin, 1.f)); // the origin is at the position of the sphere

	float A = dot(inv_dir, inv_dir);
	float B = dot(inv_origin, inv_dir);
	float C = dot(inv_origin, inv_origin) - 1; // unit sphere

	float discriminant = B * B - A * C;

	// check for the number of solutions:
	if (discriminant < 0)
		return std::make_pair(-1, glm::vec3());

	float t_result;
	// two solutions:
	// --------------------------
	if (discriminant > 0)
	{
		float sqrt_discr = sqrtf(discriminant);

		// from Physically Based Rendering: Theory to Implementation 2nd Ed.
		float q = -B - sign(B) * sqrt_discr;

		float t_h1 = q / A;
		float t_h2 = C / q;

		t_result = min(t_h1, t_h2);
		if (t_result < min_dist) // if this one doesn't work, try the other one
			t_result = max(t_h1, t_h2);
	}
	// one solution:
	// --------------------------
	else
	{
		t_result = -B / A;
	}

	// check result:
	// --------------------------
	if (t_result > min_dist && t_result < max_dist)
	{
		vec4 normal = vec4(inv_origin + (t_result * inv_dir), 0.f);

		normal = transpose(m_inverse) * normal;
		normal.w = 0;
		normal = normalize(normal); // normalize the normal

		return std::make_pair(t_result, vec3(normal));
	}
	else
	{
		return std::make_pair(-1, vec3());
	}
}

void ee::RaySphere::draw(ee::Renderer* renderer)
{
	using namespace ee;

	Shader* shader = renderer->get3Dshader();

	shader->use();
	shader->assignMat4("model", glm::value_ptr(m_trans));
	glm::mat4 cameraMat = renderer->getViewMat();
	shader->assignMat4("view", glm::value_ptr(cameraMat));
	shader->assignMat4("projection", glm::value_ptr(renderer->getProjMat()));
	shader->assignColor("defaultColor", m_color);

	m_model.draw(*shader);
}