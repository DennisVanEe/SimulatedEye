#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "shader.hpp"

namespace syn_eye
{
	class Model
	{
	public:
		Model(std::string dir);

		bool load(std::string* errorMsg = nullptr);
		void draw(Shader& shader) const;

	private:
		Mesh processMesh(const aiMesh* mesh, const aiScene* scene, bool* check = nullptr);
		void processNode(aiNode* node, const aiScene* scene, bool* check = nullptr);
		std::vector<Texture> loadTexture(const aiMaterial* mat, aiTextureType type, TextType typeName, bool* check = nullptr);

		std::vector<Mesh> m_meshes;
		std::string m_dir;
	};
}