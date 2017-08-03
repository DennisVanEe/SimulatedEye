#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/vec3.hpp>

#include <GLFW/glfw3.h>

namespace ee
{
	class Shader
	{
	public:
		void initialize(const std::string &vtxPath, const std::string &frgPath);
		~Shader();

		void use() const;

		void assignColor(std::string name, int textureUnit)
		{
			int unit = textureUnit - GL_TEXTURE0;
			glUniform1i(glGetUniformLocation(m_programID, name.c_str()), unit);
		}

		void assignMat4(std::string name, const float* mat)
		{
			int loc = glGetUniformLocation(m_programID, name.c_str());
			glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
		}

		void assignBool(std::string name, bool value)
		{
			glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
		}

		void assignColor(std::string name, glm::vec3 vec)
		{
			glUniform3f(glGetUniformLocation(m_programID, name.c_str()), vec.x / 255.f, vec.y / 255.f, vec.z / 255.f);
		}

	public:
		GLuint m_programID;
	};
}