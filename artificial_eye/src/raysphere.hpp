#pragma once

#include "glm/vec3.hpp"
#include <glm/gtx/transform.hpp>
#include "ray.hpp"

#include <utility>
#include <limits>
#include "drawable.hpp"
#include "model.hpp"
#include "type.hpp"

namespace ee
{
	class RaySphere : public Drawable
	{
	public:
		RaySphere(glm::vec3 position, glm::vec3 size, Color color = Color(255, 255, 255));
		RaySphere(glm::vec3 size, Color color = Color(255, 255, 255));
		RaySphere();

		std::pair<float, glm::vec3> intersect(Ray ray, float min_dist) const;
		inline Color getColor() const;

		void draw(ee::Renderer* renderer) override;

		glm::mat4 getTransform() const { return m_trans; }
		void setTransform(glm::mat4 transform)
		{
			m_trans = transform;
			m_inverse = glm::inverse(m_trans);
		}

	private:
		glm::mat4 m_trans;
		glm::mat4 m_inverse; // only bother if we need the inverse

		Color m_color;
		static Model m_model;
	};
}

// inline definitions

ee::Color ee::RaySphere::getColor() const
{
	return m_color;
}