#include "xwpch.h"
#include "Model.h"
#include "XuanWu/Render/Shader.h"
#include "XuanWu/Render/VertexArray.h"
#include "XuanWu/Render/RenderCommand.h"
#include "XuanWu/Render/Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace XuanWu
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Ref<Texture2D>>& textures)
		:m_vertices(vertices), m_indices(indices), m_textures(textures)
	{
		SetupMesh();
	}

	void Mesh::Draw(Ref<Shader> shader)
	{
		for (size_t i = 0; i < m_textures.size(); ++i)
		{
			m_textures[i]->Bind(i);
		}
		//shader->Bind(); // 要不要好像都可以
		RenderCommand::DrawIndexed(m_vertexArray, m_indexBuffer->GetCount());
	}

	void Mesh::SetupMesh()
	{
		m_vertexArray = VertexArray::Create();
		m_vertexBuffer = VertexBuffer::Create(&m_vertices[0], m_vertices.size() * sizeof(Vertex));
		m_vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"	},
			{ ShaderDataType::Float3, "a_Normal"	},
			{ ShaderDataType::Float2, "a_TexCoords"	}
		});
		m_vertexArray->AddVertexBuffer(m_vertexBuffer);
		m_indexBuffer = IndexBuffer::Create(&m_indices[0], m_indices.size());
		m_vertexArray->SetIndexBuffer(m_indexBuffer);
	}

	Model::Model(std::string path)
	{
		LoadModel(path);
	}

	void Model::Draw(Ref<Shader> shader)
	{
		auto s = m_meshes.size();
		for (size_t i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i].Draw(shader);
		}
	}

	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			XW_CORE_ERROR("ERROR::ASSIMP::", import.GetErrorString());
			return;
		}
		m_directory = path.substr(0, path.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.emplace_back(ProcessMesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Ref<Texture2D>> textures;

		for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;
			// 处理顶点位置
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			// 法线
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			// 纹理坐标
			// Assimp允许一个模型在一个顶点上有最多8个不同的纹理坐标，我们不会用到那么多，我们只关心第一组纹理坐标
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// 处理索引
		for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// 处理材质
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Ref<Texture2D>>&& diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Ref<Texture2D>>&& specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<Ref<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Ref<Texture2D>> textures;
		for (uint32_t i = 0; i < mat->GetTextureCount(type); ++i)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string texture_path = m_directory + "/" + str.C_Str();

			// 缓存
			bool skip = false;
			for (uint32_t j = 0; j < textures_loaded.size(); ++j)
			{
				if (textures_loaded[j]->GetPath() == texture_path)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				Ref<Texture2D> texture = Texture2D::Create(texture_path);
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}
		return textures;
	}
}