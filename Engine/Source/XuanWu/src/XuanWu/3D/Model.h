#pragma once

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace XuanWu
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Shader;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Ref<Texture2D>>& textures);
		void Draw(Ref<Shader> shader);

	public:
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<Ref<Texture2D>> m_textures;

	private:
		Ref<VertexArray> m_vertexArray;
		Ref<VertexBuffer> m_vertexBuffer;
		Ref<IndexBuffer> m_indexBuffer;
		void SetupMesh();
	};

	class Model
	{
	public:
		Model(std::string path);

		void Draw(Ref<Shader> shader);

		const std::string& GetDirectory() const { return m_directory; }
		void SetDirectory(const std::string_view dir) { m_directory = dir; }
	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	private:
		std::vector<Ref<Texture2D>> textures_loaded;

		std::vector<Mesh> m_meshes;
		std::string m_directory;
	};
}