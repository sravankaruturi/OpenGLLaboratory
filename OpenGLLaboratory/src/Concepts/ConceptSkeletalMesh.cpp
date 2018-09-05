#include "ConceptSkeletalMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <iostream>

#include <glm/gtc/matrix_transform.inl>
#include "../../external_files/ImGUI/imgui.h"

#include <GLFW/glfw3.h>

#include "../Configuration.h"

namespace olab {
	namespace concepts {

		// This just copies. This function doesn't transpose matrices.
		void convert_aimatrix_to_glm(glm::mat4& _glmMat4, const aiMatrix4x4& _aiMatrix) {

			for (auto i = 0; i < 4; i++) {
				for (auto j = 0; j < 4; j++) {
					_glmMat4[i][j] = _aiMatrix[i][j];
				}
			}

		}

		std::vector<Texture*> ConceptSkeletalMesh::LoadMaterialTextures(aiMaterial * _mat, aiTextureType _type, std::string _typeName)
		{
			std::vector<Texture *> textures;
			for (unsigned int i = 0; i < _mat->GetTextureCount(_type); i++)
			{
				aiString str;
				_mat->GetTexture(_type, i, &str);
				// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
				Texture * texture;
				texture = new Texture((std::string("Assets/Models/boblamp/") + std::string(str.C_Str())), false);
				textures.push_back(texture);
			}

			return textures;
		}

		ConceptSkeletalMesh::ConceptSkeletalMesh()
			: skinningShader("Assets/Shaders/Concept_mvp.shader"),
			position(0.0f, 0.0f, 0.0f),
			rotation(0.0f, 0.0f, 0.0f),
			scale(1.0f, 1.0f, 1.0f),
			modelMatrix(glm::mat4(1.0f)),
			worldUp(0.0f, 1.0f, 0.0f),
			fieldOfView(45.0f)
		{

			meshes.clear();

			std::cout << std::endl << "Clearing existing Meshes" << std::endl;

			cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
			cameraFront = glm::vec3(.0f, 0.0f, -1.0f);
			viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, worldUp);

			projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.f, 100.f);

#if !IS_HOME_PC
			const std::string path = "Z:/IGMProfile/Desktop/Projects/OpenGLLaboratory/OpenGLLaboratory/Assets/Models/boblamp/boblampclean.md5mesh";
#else
			const std::string path = "C:/dev/OpenGLLaboratory/OpenGLLaboratory/Assets/Models/boblamp/boblampclean.md5mesh";
#endif

			this->LoadMesh(path);

			this->position = glm::vec3(0, 0, 0);
			this->rotation = glm::vec3(90, 180, 180);
			this->scale = glm::vec3(0.2, 0.2, 0.2);

			this->cameraPosition = glm::vec3(0, 6, 22);

			//delete meshes[0].shader;

			//for (auto&it : meshes) {
			//	it.shader = &skinningShader;
			//}
		}

		ConceptSkeletalMesh::~ConceptSkeletalMesh()
		{

		}

		void ConceptSkeletalMesh::OnUpdate(float _deltaTime)
		{

			modelMatrix = glm::translate(glm::mat4(1.0f), position);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			modelMatrix = glm::scale(modelMatrix, scale);

			viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, worldUp);

			projectionMatrix = glm::perspective(glm::radians(fieldOfView), 16.0f / 9.0f, 0.1f, 100.f);

		}

		void ConceptSkeletalMesh::OnRender(const Renderer& _renderer)
		{

			Shader * using_shader = meshes[0].shader;

			using_shader->use();
			using_shader->setInt("u_Texture", 0);
			using_shader->setMat4("u_ModelMatrix", modelMatrix);
			using_shader->setMat4("u_ViewMatrix", viewMatrix);
			using_shader->setMat4("u_ProjectionMatrix", projectionMatrix);

		}

		void ConceptSkeletalMesh::OnImGuiRender()
		{
			
			ImGui::Text("Model Matrix");
			ImGui::InputFloat3("Position", glm::value_ptr(position));
			ImGui::InputFloat3("Rotation", glm::value_ptr(rotation));
			ImGui::InputFloat3("Scale", glm::value_ptr(scale));
			ImGui::Separator();
			ImGui::Text("View Matrix");
			ImGui::SliderFloat3("Camera Position", glm::value_ptr(cameraPosition), 0.5f, 50.0f, "%.3f", 2.0f);
			ImGui::Separator();
			ImGui::Text("Projection Matrix");
			ImGui::SliderFloat("Field Of View", &fieldOfView, 0.5f, 90.0f, "%.3f", 2.0f);

		}

		void ConceptSkeletalMesh::LoadMesh(std::string _path)
		{
			Assimp::Importer import;
			const aiScene *scene = import.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
				return;
			}
			std::string directory = _path.substr(0, _path.find_last_of('/'));

			this->ProcessNode(scene->mRootNode, scene);

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

		void ConceptSkeletalMesh::ProcessNode(aiNode * _node, const aiScene * _scene)
		{
			// process all the node's meshes (if any)
			for (unsigned int i = 0; i < _node->mNumMeshes; i++)
			{
				aiMesh *mesh = _scene->mMeshes[_node->mMeshes[i]];
				meshes.push_back(this->ProcessMesh(mesh, _scene));
			}
			// then do the same for each of its children
			for (unsigned int i = 0; i < _node->mNumChildren; i++)
			{
				this->ProcessNode(_node->mChildren[i], _scene);
			}

		}

		ConceptSkeletalMesh::Mesh ConceptSkeletalMesh::ProcessMesh(aiMesh * _mesh, const aiScene * _scene)
		{
			Mesh return_mesh;

			std::vector<float> vertices;
			std::vector<unsigned int> indices;

			for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
			{
				float tex_x = 0.0f;
				float tex_y = 0.0f;

				if (_mesh->mTextureCoords[0])
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

			// A Bone or bones for each mesh.
			for (auto i = 0; i < _mesh->mNumBones; i++) {

				auto bone_index = 0;
				std::string bone_name(_mesh->mBones[i]->mName.data);

				// We could not find the bone in our map. Add it and add it to the Bone Info Data
				if (boneMapping.find(bone_name) == boneMapping.end()) {

					// TODO: If some thing doesn't work, maybe store this in a separate variable like he does.
					bone_index = boneMapping.size();

					BoneInfo bi;
					// Since the GLM Matrices are Column Major, 
					convert_aimatrix_to_glm(bi.boneOffset, _mesh->mBones[i]->mOffsetMatrix);
					
					boneInfoData.push_back(bi);

					boneMapping[bone_name] = bone_index;

				}
				else {
					bone_index = boneMapping[bone_name];
				}

				for (auto j = 0; j < _mesh->mBones[i]->mNumWeights; j++) {
					// Since we separate each mesh, the base vertex would be 0. ??
					auto vertex_id = 0 + _mesh->mBones[i]->mWeights[j].mVertexId;
					auto weight = _mesh->mBones[i]->mWeights[j].mWeight;
					this->vertexBoneData[vertex_id].AddBoneData(bone_index, weight);
				}

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
			const std::vector<Texture *> diffuseMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

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

		void VertexBoneData::AddBoneData(unsigned int _boneID, float _weight)
		{
			for (auto i = 0; i < sizeof(Ids)/sizeof(unsigned int); i++) {
				if (weights[i] == 0.0) {
					Ids[i] = _boneID;
					weights[i] = _weight;
					return;
				}
			}

			// should never get here - more bones than we have space for
			assert(0);
		}

	}
}