#pragma once

#include "glm/vec3.hpp"

struct Ray
{
	glm::vec3 dir;
	glm::vec3 origin;

	Ray(glm::vec3 d, glm::vec3 o) : dir(d), origin(o) {}
	Ray() {}
};