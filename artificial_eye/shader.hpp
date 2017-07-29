#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace syn_eye
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

	public:
		GLuint m_programID;
	};
}