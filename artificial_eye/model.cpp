#include "model.hpp"

#include <stdexcept>
#include <iostream>
#include "textloading.hpp"

using namespace std;
using namespace glm;

syn_eye::Model::Model(std::string dir) :
	m_dir(dir)
{
}

void syn_eye::Model::draw(Shader& shader) const
{
	for (const Mesh& m : m_meshes)
	{
		m.draw(shader);
	}
}

bool syn_eye::Model::load(string* errorMsg)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_dir, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		if (errorMsg)
		{
			// set the error message:
			errorMsg->assign(importer.GetErrorString());
		}
		return false;
	}

	auto pos = m_dir.find_last_of('/');
	if (pos == string::npos)
	{
		m_dir = "/";
	}
	else
	{
		m_dir = m_dir.substr(0, pos) + "/";
	}

	aiNode* node = scene->mRootNode;
	bool loadCheck;
	processNode(node, scene, &loadCheck);
	if (!loadCheck)
	{
		errorMsg->assign("Error when parsing and setting up nodes.");
		return false;
	}

	return true;
}

vector<syn_eye::Texture> syn_eye::Model::loadTexture(const aiMaterial* mat, aiTextureType type, TextType typeName, bool* check)
{
	using namespace syn_eye;
	vector<Texture> textures;
	for (size_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;

		texture.id = TextureFromFile(str.C_Str(), m_dir);
		if (texture.id == 0 && check)
		{
			*check = false;
			return vector<Texture>();
		}

		texture.type = typeName;
		textures.push_back(texture);
	}

	*check = true;
	return textures;
}

syn_eye::Mesh syn_eye::Model::processMesh(const aiMesh* mesh, const aiScene* scene, bool* check)
{
	vector<Vertex> tempVert;
	vector<GLuint> tempInd;
	vector<Texture> tempTexts;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.norm.x = mesh->mNormals[i].x;
		vertex.norm.z = mesh->mNormals[i].y;
		vertex.norm.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.textcoord.x = mesh->mTextureCoords[0][i].x;
			vertex.textcoord.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.textcoord = vec2(0.f, 0.f);
		}

		tempVert.push_back(vertex);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace* face = &mesh->mFaces[i];
		for (size_t k = 0; k < face->mNumIndices; k++)
		{
			tempInd.push_back(face->mIndices[k]);
		}
	}

	bool correctLoad;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuse = loadTexture(material, aiTextureType_DIFFUSE, TextType::DIFFUSE, &correctLoad);
		if (!correctLoad && check)
		{
			*check = false;
			return Mesh();
		}
		tempTexts.insert(tempTexts.end(), diffuse.begin(), diffuse.end());
		vector<Texture> specular = loadTexture(material, aiTextureType_SPECULAR, TextType::SPECULAR, &correctLoad);
		if (!correctLoad && check)
		{
			*check = false;
			return Mesh();
		}
		tempTexts.insert(tempTexts.end(), specular.begin(), specular.end());
	}

	Mesh result;
	if (!result.loadValues(tempVert, tempInd, tempTexts))
	{
		*check = false;
		return Mesh();
	}

	*check = true;
	return result;
}

void syn_eye::Model::processNode(aiNode* node, const aiScene* scene, bool* check)
{
	bool correctLoad;
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene, &correctLoad));
		if (!correctLoad && check)
		{
			*check = false;
		}
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, &correctLoad);
		if (!correctLoad && check)
		{
			*check = false;
		}
	}

	*check = true;
}