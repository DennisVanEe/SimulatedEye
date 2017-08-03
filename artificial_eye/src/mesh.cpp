#include "mesh.hpp"

using namespace std;

const std::string MATERIAL_NAME = ""; // or "" if nothing
const std::string DIFFUSE_NAME = "textDiffuse";
const std::string SPECULAR_NAME = "textSpec";
const std::string DIFFUSE_CHECK_NAME = "useTD";

bool ee::Mesh::loadValues(const vector<Vertex>& vertices, const vector<GLuint>& indices, const vector<Texture>& textures)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;

	GLuint m_VBO, m_EBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textcoord));

	glBindVertexArray(0);

	return true;
}

void ee::Mesh::draw(Shader& shader) const
{
	unsigned diffuseCnt = 0;
	unsigned specularCnt = 0;
	for (size_t i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		if (m_textures[i].type == TextType::DIFFUSE)
		{
			shader.assignColor(MATERIAL_NAME + DIFFUSE_NAME + std::to_string(i), GL_TEXTURE0 + i);
			shader.assignBool(DIFFUSE_CHECK_NAME + std::to_string(i), true);
		}
		else if (m_textures[i].type == TextType::SPECULAR)
		{
			shader.assignColor(MATERIAL_NAME + SPECULAR_NAME + std::to_string(i), GL_TEXTURE0 + i);
		}
		else assert(false); // this shouldn't happen, so, yeah.

		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}

	if (m_textures.size() == 0)
		shader.assignBool("noText", true);
	else
		shader.assignBool("noText", false);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}