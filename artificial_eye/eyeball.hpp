#pragma once

#include "imageplane.hpp"
#include "world.hpp"

#include <glm/vec3.hpp>
#include "raysphere.hpp"
#include "camera.hpp"
#include "Model.hpp"
#include "drawable.hpp"
#include "renderer.hpp"

#include <string>

const float HORIZONTAL_FOV = 200.f;
const float VERTICAL_FOV = 100.f;

namespace syn_eye
{
	class EyeBall : public Drawable
	{
	public:
		// widthPercent: number form 0 to 1, percent of semicircle.
		// heightPercent: number from 0 to 1, percent of semicircle.
		EyeBall(glm::vec3 pos, float radius, float retinaWidth, float retinaHeight, ImagePlane* image, World* world);

		void transform(glm::mat4 trans)
		{
			m_transform = trans;
		}
		glm::mat4 getTransform() const
		{
			return m_transform;
		}

		void calculateImage();

		void draw(Renderer* shader) override;

	private:
		// eyeball geometry properties:
		glm::mat4 m_transform; // cache the transformation

		// used when doing calculations:
		float m_startWidth;
		float m_startHeight;
		float m_incWidth;
		float m_incHeight;

		// eyeball rendering properties:
		ImagePlane* m_imageBuffer;  // needed to write the result to
		World* m_world;				// needed to ray trace with surroundings
		Model m_model;				// the thing that will get drawn		

		// some helper functions:
		Color rayTrace(Ray ray);
	};
}