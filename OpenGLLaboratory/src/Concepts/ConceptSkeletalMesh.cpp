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

		ConceptSkeletalMesh::ConceptSkeletalMesh()
			: position(0.0f, 0.0f, 0.0f),
			rotation(0.0f, 0.0f, 0.0f),
			scale(1.0f, 1.0f, 1.0f),
			modelMatrix(glm::mat4(1.0f)),
			worldUp(0.0f, 1.0f, 0.0f),
			fieldOfView(45.0f)
		{

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

			skinningShader = new olab::Shader("Assets/Shaders/Concept_skinning.shader");

			model = new SkeletalModel(path, skinningShader);

			this->position = glm::vec3(0, 0, 0);
			this->rotation = glm::vec3(90, 180, 180);
			this->scale = glm::vec3(0.2, 0.2, 0.2);

			this->cameraPosition = glm::vec3(0, 6, 22);

		}

		ConceptSkeletalMesh::~ConceptSkeletalMesh()
		{
			delete model;
		}

		void ConceptSkeletalMesh::OnUpdate(float _deltaTime)
		{

			model->Update(_deltaTime);

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

			Shader * using_shader = model->shader;

			using_shader->use();
			using_shader->setInt("u_Texture", 0);
			using_shader->setMat4("u_ModelMatrix", modelMatrix);
			using_shader->setMat4("u_ViewMatrix", viewMatrix);
			using_shader->setMat4("u_ProjectionMatrix", projectionMatrix);

			this->model->Render(_renderer);

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

		SkeletalModel::SkeletalModel(const std::string & _filename, Shader * _shader)
		{
			filename = _filename;
			this->shader = _shader;
			this->LoadModel(this->filename);

		}

		SkeletalModel::~SkeletalModel()
		{

		}

		void SkeletalModel::LoadModel(const std::string & _filename)
		{

			std::string directory = _filename.substr(0, _filename.find_last_of('/'));

			this->ProcessModel();

			this->shader->use();
			this->shader->setMat4("u_ModelMatrix", glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
			this->shader->setMat4("u_ViewMatrix", glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
			this->shader->setMat4("u_ProjectionMatrix", glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f));
			
		}

		void SkeletalModel::Render(const Renderer& _renderer)
		{
			// Render all the Meshes.
			for (const auto& it : meshes) {

				shader->use();
				shader->setInt("u_Texture", 0);

				// Get the Bone Matrices from the bone info.
				std::vector<glm::mat4> bone_matrices;
				this->BoneTransform(0.0f, bone_matrices);

				auto loc = shader->getUniformLocation("u_BoneMatrices");
				// We transpose the matrices here because ASSIMP matrices are row major.
				GLCall(glUniformMatrix4fv(loc, bone_matrices.size(), GL_FALSE, &bone_matrices[0][0][0]));

				if (it.textures.size() > 0) {
					it.textures[0]->Bind(0);
				}

				if (nullptr == it.va || nullptr == it.ib)
				{
					__debugbreak();
				}

				it.vb->Bind();
				_renderer.Draw(it.va, it.ib, shader);

			}

		}

		void SkeletalModel::Update(float _deltatime)
		{
			// We first update just the Bone Matrices.
		}

		void SkeletalModel::SetShowSkeleton(bool _showSkeleton)
		{
			showSkeleton = _showSkeleton;
		}

		void SkeletalModel::ProcessModel()
		{

			Assimp::Importer import;
			const aiScene *scene = import.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
				return;
			}

			numberOfMeshes = 0;

			// Store the number of Vertices.
			numberOfVertices = 0;
			for (auto i = 0; i < scene->mNumMeshes; i++) {
				numberOfVertices += scene->mMeshes[i]->mNumVertices;
			}

			// Resize the Bone Vertex Data as such.
			bones.resize(numberOfVertices);

			// Cycle through all the meshes. Since all the meshes are part of the aiScene, it shouldn't be a big deal.
			numberOfMeshes = scene->mNumMeshes;
			meshes.resize(numberOfMeshes);

			std::vector<VertexData> vertices;
			std::vector<unsigned int> indices;

			// This is used to get the vertices that are pointed to, by the bones.
			auto base_vertex_index = 0;
			const auto vertex_size = 5;

			for (auto i = 0; i < numberOfMeshes; i++) {

				vertices.clear();
				indices.clear();

				aiMesh * current_mesh = scene->mMeshes[i];

				vertices.resize(current_mesh->mNumVertices);

				for (auto j = 0; j < current_mesh->mNumVertices; j++) {

					
					// Push each vertex onto the mesh.

					float tex_x = 0.0f;
					float tex_y = 0.0f;

					if (scene->mMeshes[i]->mTextureCoords[0]) {
						tex_x = current_mesh->mTextureCoords[0][j].x;
						tex_y = current_mesh->mTextureCoords[0][j].y;
					}

					vertices[j].position[0] = current_mesh->mVertices[j].x;
					vertices[j].position[1] = current_mesh->mVertices[j].y;
					vertices[j].position[2] = current_mesh->mVertices[j].z;

					//if (current_mesh->HasNormals()) {
					//	vertices.push_back(current_mesh->mNormals[j].x);
					//	vertices.push_back(current_mesh->mNormals[j].y);
					//	vertices.push_back(current_mesh->mNormals[j].z);
					//}
					//else {
					//	vertices.push_back(0.0f);
					//	vertices.push_back(0.0f);
					//	vertices.push_back(0.0f);
					//}
					

					vertices[j].texCoord[0] = tex_x;
					vertices[j].texCoord[1] = tex_y;

				}

				// Load all the Bones for this particular AiMesh
				{

					for (auto j = 0; j < current_mesh->mNumBones; j++) {

						auto bone_index = 0;
						std::string bone_name = std::string(current_mesh->mBones[j]->mName.data);

						if (boneMapping.find(bone_name) == boneMapping.end()) {

							// The Bone is not there in the mapping. It means, it is a new bone.
							// Create the new bone and add it to the mapping.

							bone_index = numberOfBones;
							numberOfBones++;

							BoneInfo bi;
							boneInfoData.push_back(bi);

							// We set the bone offset matrix
							convert_aimatrix_to_glm(boneInfoData[bone_index].boneOffset, current_mesh->mBones[j]->mOffsetMatrix);

							// Add that to the mapping
							boneMapping[bone_name] = bone_index;

						}
						else {
							bone_index = boneMapping[bone_name];
						}

						for (auto k = 0; k < current_mesh->mBones[j]->mNumWeights; k++) {

							unsigned int vertex_id = base_vertex_index + current_mesh->mBones[j]->mWeights[k].mVertexId;
							unsigned int local_vertex_id = current_mesh->mBones[j]->mWeights[k].mVertexId;
							float weight = current_mesh->mBones[j]->mWeights[k].mWeight;

							bones[vertex_id].AddBoneData(bone_index, weight);
							if (bone_index > 32) {
								__debugbreak();
							}
							vertices[local_vertex_id].vbd = bones[vertex_id];

						}

					}

					//// Once you load all the Data into the bones variable, move them to the Vertices variable ??
					//for (auto m = 0; m < current_mesh->mNumVertices; m++) {

					//	

					//}

				}


				for (auto j = 0; j < current_mesh->mNumFaces; j++) {

					aiFace face = current_mesh->mFaces[j];

					for (auto k = 0; k < face.mNumIndices; k++) {
						indices.push_back(face.mIndices[k]);
					}

				}

				// Deal with the materials now.
				aiMaterial * material = scene->mMaterials[current_mesh->mMaterialIndex];
				const std::vector<Texture *> diffuse_maps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

				VertexBuffer * vb = new VertexBuffer(&vertices[0], sizeof(VertexData) * vertices.size());

				VertexBufferLayout vbl;
				vbl.Push<float>(3);	// Pos
				//vbl.Push<float>(3);	// Nor
				vbl.Push<float>(2);	// Tex

				vbl.Push<unsigned int>(4);	// Bone Index
				vbl.Push<float>(4);	// Bone Weight

				meshes[i].vb = vb;
				meshes[i].va = new VertexArray();
				meshes[i].va->AddBuffer(*vb, vbl);
				meshes[i].ib = new IndexBuffer(&indices[0], indices.size());
				meshes[i].textures = diffuse_maps;

				base_vertex_index += current_mesh->mNumVertices;

			}

			shader->use();
			shader->setMat4("u_ModelMatrix", glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
			shader->setMat4("u_ViewMatrix", glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
			shader->setMat4("u_ProjectionMatrix", glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f));

		}

		const std::vector<Texture *> SkeletalModel::LoadMaterialTextures(aiMaterial * _material, aiTextureType _textureType, std::string _param3)
		{
			std::vector<Texture *> textures;
			for (unsigned int i = 0; i < _material->GetTextureCount(_textureType); i++)
			{
				aiString str;
				_material->GetTexture(_textureType, i, &str);
				// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
				Texture * texture;
				texture = new Texture((std::string("Assets/Models/boblamp/") + std::string(str.C_Str())), false);
				textures.push_back(texture);
			}

			return textures;
		}

		void SkeletalModel::BoneTransform(float _deltatime, std::vector<glm::mat4>& _matrices)
		{

			_matrices.resize(numberOfBones);

			// For now, set them to Identity.
			for (auto& it : _matrices) {
				it = glm::mat4(1.0f);
			}

		}

}
}