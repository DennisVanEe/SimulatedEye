#pragma once

#include "glm/vec3.hpp"
#include <glm/gtx/transform.hpp>
#include "ray.hpp"

#include <utility>
#include <limits>
#include "drawable.hpp"
#include "model.hpp"

class RaySphere : public syn_eye::Drawable
{
public:
	RaySphere(glm::vec3 position, glm::vec3 size);
	RaySphere(glm::vec3 size);
	RaySphere();

	std::pair<float, glm::vec3> intersect(Ray ray, float min_dist) const;

	void draw(syn_eye::Renderer* renderer) override;

private:
	glm::mat4 m_trans;
	glm::mat4 m_inverse; // only bother if we need the inverse

	syn_eye::Model m_sphere;
};