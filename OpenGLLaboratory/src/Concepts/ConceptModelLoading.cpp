#include "ConceptModelLoading.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <iostream>

#include "../IndexBuffer.h"
#include "../VertexBuffer.h"
#include "../Shader.h"
#include <glm/gtc/matrix_transform.inl>

#include "../Configuration.h"

namespace olab
{
	namespace concepts
	{
		
		ConceptModelLoading::Mesh ConceptModelLoading::ProcessMesh(aiMesh *_mesh, const aiScene *_scene)
		{
			Mesh return_mesh;

			std::vector<float> vertices;
			std::vector<unsigned int> indices;


			for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
			{
				float tex_x = 0.0f;
				float tex_y = 0.0f;

				if ( _mesh->mTextureCoords[0] )
				{
					tex_x = _mesh->mTextureCoords[0][i].x;
					tex_y = _mesh->mTextureCoords[0][i].y;
				}

				vertices.push_back(_mesh->mVertices[i].x);
				vertices.push_back(_mesh->mVertices[i].y);
				vertices.push_back(_mesh->mVertices[i].z);

				//vertices.push_back(_mesh->mNormals[i].x);
				//vertices.push_back(_mesh->mNormals[i].y);
				//vertices.push_back(_mesh->mNormals[i].z);

				vertices.push_back(tex_x);
				vertices.push_back(tex_y);
			}

			// process indices
			for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
			{
				aiFace face = _mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}

			aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];
			const std::vector<Texture *> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

			const unsigned int __temp = sizeof(float) * vertices.size();

			VertexBuffer * vb = new VertexBuffer(&vertices[0], __temp);
			VertexBufferLayout vbl;
			vbl.Push<float>(3);
			//vbl.Push<float>(3);
			vbl.Push<float>(2);

			return_mesh.vb = vb;
			return_mesh.va = new VertexArray();
			return_mesh.va->AddBuffer(*vb, vbl);
			return_mesh.ib = new olab::IndexBuffer(&indices[0], indices.size());
			return_mesh.textures = diffuseMaps;

			return return_mesh;
		}

		void ConceptModelLoading::ProcessNode(aiNode *_node, const aiScene *_scene)
		{
			// process all the node's meshes (if any)
			for (unsigned int i = 0; i < _node->mNumMeshes; i++)
			{
				aiMesh *mesh = _scene->mMeshes[_node->mMeshes[i]];
				meshes.push_back(ProcessMesh(mesh, _scene));
			}
			// then do the same for each of its children
			for (unsigned int i = 0; i < _node->mNumChildren; i++)
			{
				ProcessNode(_node->mChildren[i], _scene);
			}
		}

		// checks all material textures of a given type and loads the textures if they're not loaded yet.
			// the required info is returned as a Texture struct.
		std::vector<Texture *> ConceptModelLoading::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
		{
			std::vector<Texture *> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
				Texture * texture;
				//texture = new Texture((std::string("Assets/Models/boblamp/") + std::string(str.C_Str())), false);
				texture = new Texture(std::string(directory + '/' + str.C_Str()), false);
				textures.push_back(texture);
			}

			return textures;
		}

		ConceptModelLoading::ConceptModelLoading()
		{

#if IS_HOME_PC
			//std::string path = "C:/dev/OpenGLLaboratory/OpenGLLaboratory/Assets/Models/nanosuit/nanosuit.obj";
			std::string path = "C:/Users/Sravan\ Karuturi/Desktop/Blender\ Exports/BendyCube.dae";
#else 
			std::string path = "Z:/IGMProfile/Desktop/Projects/OpenGLLaboratory/OpenGLLaboratory/Assets/Models/nanosuit/nanosuit.obj";
#endif
			this->LoadMesh(path);
			
		}

		ConceptModelLoading::~ConceptModelLoading()
		{
			// Delete all the memory allocated for each mesh.
			for (auto& it : meshes) {
				delete it.ib;
				for (auto t_it : it.textures) {
					delete t_it;
				}
				delete it.va;
				delete it.vb;
			}

			// We only have one instance of it.
			delete meshes[0].shader;
		}

		void ConceptModelLoading::LoadMesh(std::string _path)
		{

			Assimp::Importer import;
			const aiScene *scene = import.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
				return;
			}
			directory = _path.substr(0, _path.find_last_of('/'));

			ProcessNode(scene->mRootNode, scene);

			Shader * shader = new Shader("Assets/Shaders/Concept_mvp.shader");

			shader->use();
			shader->setMat4("u_ModelMatrix", glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
			shader->setMat4("u_ViewMatrix", glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
			shader->setMat4("u_ProjectionMatrix", glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f));

			for (auto& it : meshes)
			{
				it.shader = shader;
			}

		}

		void ConceptModelLoading::OnRender(const Renderer& _renderer)
		{
			for (const auto& it : meshes)
			{
				it.shader->use();
				it.shader->setInt("u_Texture", 0);
				if ( it.textures.size() > 0)
				{
					it.textures[0]->Bind(0);
				}

				if ( nullptr == it.va || nullptr == it.ib || nullptr == it.shader)
				{
					__debugbreak();
				}
				it.vb->Bind();
				_renderer.Draw(it.va, it.ib, it.shader);
			}
		}

		void ConceptModelLoading::OnImGuiRender()
		{
			
		}

	}
}
