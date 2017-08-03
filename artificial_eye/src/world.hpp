#pragma once

#include <vector>
#include "raysphere.hpp"

namespace ee
{
	class World
	{
	public:
		void addSphere(RaySphere sphere)
		{
			m_spheres.push_back(sphere);
		}
		std::vector<RaySphere>& getSpheres()
		{
			return m_spheres;
		}

	private:
		std::vector<RaySphere> m_spheres;
	};
}