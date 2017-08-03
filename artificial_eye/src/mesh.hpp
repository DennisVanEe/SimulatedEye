#pragma once

#include <assimp/Importer.hpp>
#include <string>
#include <glad/glad.h>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "shader.hpp"

namespace ee
{
    // note: in this case, specular is not going to be used because there
    // is no light source
	enum class TextType {DIFFUSE, SPECULAR, NONE};

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 norm;
		glm::vec2 textcoord;
	};

	struct Texture
	{
		GLuint id;
		float opacity;
		TextType type;

		Texture() {}
		Texture(GLuint i, float o, TextType t) :
			id(i), opacity(o), type(t) {}
	};

	class Mesh
	{
	public:
		inline Mesh();
		inline ~Mesh();

		bool loadValues(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& texture);

		void draw(Shader& shader) const;

	private:
		std::vector<Vertex> m_vertices;
		std::vector<GLuint> m_indices;
		std::vector<Texture> m_textures;

		GLuint m_VAO;
	};
}

// inline definitions:

ee::Mesh::Mesh()
{
}

ee::Mesh::~Mesh()
{
	// glDeleteVertexArrays(1, &m_VAO);
}