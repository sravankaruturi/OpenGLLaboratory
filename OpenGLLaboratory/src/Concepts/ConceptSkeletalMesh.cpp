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

		void display_matrix_in_imgui(const glm::mat4& _mat) {
			for (auto i = 0; i < 4; i++) {
				for (auto j = 0; j < 4; j++) {
					ImGui::Text("%.2f ", _mat[i][j]);
					ImGui::SameLine();
				}
				ImGui::Text("\n");
			}
		}

		// This just copies. This function doesn't transpose matrices.
		void convert_aimatrix_to_glm(glm::mat4& _glmMat4, const aiMatrix4x4& _aiMatrix) {

			for (auto i = 0; i < 4; i++) {
				for (auto j = 0; j < 4; j++) {
					_glmMat4[i][j] = _aiMatrix[i][j];
				}
			}

		}

		// This just copies. This function doesn't transpose matrices.
		void convert_aimatrix_to_glm(glm::mat4& _glmMat4, const aiMatrix3x3& _aiMatrix) {

			for (auto i = 0; i < 3; i++) {
				for (auto j = 0; j < 3; j++) {
					_glmMat4[i][j] = _aiMatrix[i][j];
				}
			}

			// The rest would be zero, other than the 4,4.
			_glmMat4[0][3] = 0.0f;
			_glmMat4[1][3] = 0.0f;
			_glmMat4[2][3] = 0.0f;

			_glmMat4[3][0] = 0.0f;
			_glmMat4[3][1] = 0.0f;
			_glmMat4[3][2] = 0.0f;

			_glmMat4[3][3] = 1.0f;

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

			auto total_time = glfwGetTime();

			this->model->Render(total_time, _renderer);

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

			ImGui::Separator();

			if (ImGui::Button( (!showBoneWindow) ? "Show Bone mapping" : "Hide Bone Mapping")) {
				this->showBoneWindow = !showBoneWindow;
			}

			if (ImGui::Button((!showBoneInfoWindow) ? "Show Bone Info" : "Hide Bone Info")) {
				this->showBoneInfoWindow = !showBoneInfoWindow;
			}

			if (ImGui::Button((!showBoneDataPerMesh) ? "Show Vertex Data for the Lamp" : "Hide Vertex Data for the Lamp")) {
				this->showBoneDataPerMesh = !showBoneDataPerMesh;
			}

			if (this->showBoneInfoWindow) {

				ImGui::Begin("Bone Info", &showBoneInfoWindow);

				// We can display the Matrices for each Bone I guess.
				for (const auto& it : this->model->boneInfoData) {
					display_matrix_in_imgui(it.boneOffset);
					ImGui::Separator();
				}

				ImGui::End();

			}

			if (this->showBoneWindow) {

				ImGui::Begin("Bone Mapping", &showBoneWindow);

				ImGui::Text("Bone/Joints Debug Data");
				ImGui::Text("Bones: ");
				// Now for each Joint, we display the data. Stored, in BoneInfo
				for (const auto& it : this->model->boneMapping) {
					ImGui::Text((it.first + std::string(" : ") + std::to_string(it.second)).c_str());
				}

				ImGui::End();

			}

			if (this->showBoneDataPerMesh) {

				ImGui::Begin("Bone Debug for Lamp", &showBoneDataPerMesh);

				// Show all the meshes in the Model and I'll click on what I want.
				auto temp_index = 0;
				for (auto& it : this->model->meshes) {

					std::string name = "Select Mesh - " + std::to_string(temp_index);
					if (ImGui::Button(name.c_str())) {
						selected_mesh = &it;
					}
					temp_index++;

				}

				if (nullptr != selected_mesh) {

					// Display the Bone Vertex Data
					for (const auto& itit : selected_mesh->vertexBoneData) {
						// For Each Vertex, Display the Data.
						// This is Dumb

						ImGui::Text("%d\t %d\t %d\t %d\t", itit.Ids[0], itit.Ids[1], itit.Ids[2], itit.Ids[3]);
						ImGui::Text("%.3f\t %.3f\t %.3f\t %.3f\t", itit.weights[0], itit.weights[1], itit.weights[2], itit.weights[3] );
						ImGui::Separator();

					}
				}

				ImGui::End();

			}

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

		unsigned int SkeletalModel::FindScaling(float _animationTime, const aiNodeAnim* _nodeAnim)
		{
			assert(_nodeAnim->mNumScalingKeys > 0);
			for (unsigned int i = 0; i < _nodeAnim->mNumScalingKeys - 1; i++) {
				if (_animationTime < (float)_nodeAnim->mScalingKeys[i + 1].mTime) {
					return i;
				}
			}

			// It should never reach here.
			assert(0);
			return 0;
		}

		unsigned int SkeletalModel::FindRotation(float _animationTime, const aiNodeAnim* _nodeAnim)
		{
			assert(_nodeAnim->mNumRotationKeys> 0);
			for (unsigned int i = 0; i < _nodeAnim->mNumRotationKeys - 1; i++) {
				if (_animationTime < (float)_nodeAnim->mRotationKeys[i + 1].mTime) {
					return i;
				}
			}

			// It should never reach here.
			assert(0);
			return 0;
		}

		unsigned int SkeletalModel::FindPosition(float _animationTime, const aiNodeAnim* _nodeAnim)
		{
			assert(_nodeAnim->mNumPositionKeys > 0);
			for (unsigned int i = 0; i < _nodeAnim->mNumPositionKeys- 1; i++) {
				if (_animationTime < (float)_nodeAnim->mPositionKeys[i + 1].mTime) {
					return i;
				}
			}

			// It should never reach here.
			assert(0);
			return 0;
		}

		void SkeletalModel::CalcInterpolatedPosition(aiVector3D& _out, float _animationTime, const aiNodeAnim* _nodeAnim)
		{
			if (_nodeAnim->mNumPositionKeys == 1) {
				// There is only one Position.
				_out = _nodeAnim->mPositionKeys[0].mValue;
				return;
			}

			unsigned int position_index = FindPosition(_animationTime, _nodeAnim);
			unsigned int next_pos_index = position_index + 1;
			assert(next_pos_index < _nodeAnim->mNumPositionKeys);

			// The Difference between two key frames.
			float delta_time = (float)(_nodeAnim->mPositionKeys[next_pos_index].mTime - _nodeAnim->mPositionKeys[position_index].mTime);

			// The Factor by which the current frame has transitioned into the next frame.
			float factor = (_animationTime - (float)_nodeAnim->mPositionKeys[position_index].mTime) / delta_time;

			assert(factor >= 0.0f && factor <= 1.0f);

			const auto start = _nodeAnim->mPositionKeys[position_index].mValue;
			const auto end = _nodeAnim->mPositionKeys[next_pos_index].mValue;

			_out = start + factor * (end - start);

		}

		void SkeletalModel::CalcInterpolatedRotation(aiQuaternion& _out, float _animationTime, const aiNodeAnim* _nodeAnim)
		{

			if (_nodeAnim->mNumRotationKeys == 1) {
				// There is only one Position.
				_out = _nodeAnim->mRotationKeys[0].mValue;
				return;
			}

			unsigned int rotation_index = FindRotation(_animationTime, _nodeAnim);
			unsigned int next_rot_index = rotation_index + 1;
			assert(next_rot_index < _nodeAnim->mNumRotationKeys);

			// The Difference between two key frames.
			float delta_time = (float)(_nodeAnim->mRotationKeys[next_rot_index].mTime - _nodeAnim->mRotationKeys[rotation_index].mTime);

			// The Factor by which the current frame has transitioned into the next frame.
			float factor = (_animationTime - (float)_nodeAnim->mRotationKeys[rotation_index].mTime) / delta_time;

			assert(factor >= 0.0f && factor <= 1.0f);

			const auto start = _nodeAnim->mRotationKeys[rotation_index].mValue;
			const auto end = _nodeAnim->mRotationKeys[next_rot_index].mValue;

			aiQuaternion::Interpolate(_out, start, end, factor);

			_out = _out.Normalize();

		}

		void SkeletalModel::CalcInterpolatedScaling(aiVector3D& _out, float _animationTime, const aiNodeAnim* _nodeAnim)
		{

			if (_nodeAnim->mNumScalingKeys == 1) {
				_out = _nodeAnim->mScalingKeys[0].mValue;
				return;
			}

			auto scaling_index = FindScaling(_animationTime, _nodeAnim);
			auto nex_sca_index = scaling_index + 1;

			assert(nex_sca_index < _nodeAnim->mNumScalingKeys);

			auto delta_time = (float)(_nodeAnim->mScalingKeys[nex_sca_index].mTime - _nodeAnim->mScalingKeys[scaling_index].mTime);

			auto factor = (_animationTime - (float)_nodeAnim->mScalingKeys[scaling_index].mTime) / delta_time;

			assert(factor >= 0.0f && factor <= 1.0f);

			auto start = _nodeAnim->mScalingKeys[scaling_index].mValue;
			auto end = _nodeAnim->mScalingKeys[nex_sca_index].mValue;

			_out = start + factor * (end - start);

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

		void SkeletalModel::Render(float _totalTime, const Renderer& _renderer)
		{
			// Render all the Meshes.
			for (const auto& it : meshes) {

				shader->use();
				shader->setInt("u_Texture", 0);

				// Get the Bone Matrices from the bone info.
				std::vector<glm::mat4> bone_matrices;
				this->BoneTransform(_totalTime, bone_matrices);

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

			this->scene = import.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
				return;
			}

			convert_aimatrix_to_glm(globalInverseTransform, scene->mRootNode->mTransformation);

			globalInverseTransform = glm::inverse(globalInverseTransform);

			numberOfMeshes = 0;

			// Store the number of Vertices.
			numberOfVertices = 0;
			for (auto i = 0; i < scene->mNumMeshes; i++) {
				numberOfVertices += scene->mMeshes[i]->mNumVertices;
			}

			// Cycle through all the meshes. Since all the meshes are part of the aiScene, it shouldn't be a big deal.
			numberOfMeshes = scene->mNumMeshes;
			meshes.resize(numberOfMeshes);

			std::vector<VertexData> vertices;
			std::vector<unsigned int> indices;
			std::vector<olab::concepts::VertexBoneData> boneWeights;

			// This is used to get the vertices that are pointed to, by the bones.
			auto base_vertex_index = 0;
			const auto vertex_size = 5;

			for (auto i = 0; i < numberOfMeshes; i++) {

				vertices.clear();
				indices.clear();
				boneWeights.clear();

				aiMesh * current_mesh = scene->mMeshes[i];

				vertices.resize(current_mesh->mNumVertices);
				boneWeights.resize(current_mesh->mNumVertices);

				for (auto& it : boneWeights) {
					it.Reset();
				}

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

							boneWeights[local_vertex_id].AddBoneData(bone_index, weight);
							if (bone_index > 32) {
								__debugbreak();
							}
							//vertices[local_vertex_id].vbd = boneWeights[local_vertex_id];

						}

					}

					// Once you load all the Data into the bones variable, move them to the Vertices variable ??
					for (auto m = 0; m < current_mesh->mNumVertices; m++) {

						vertices[m].vbd = boneWeights[m];

					}

					meshes[i].vertexBoneData = boneWeights;

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

				// Check for stupid bone indices
				{
					for (auto temp = 0; temp < vertices.size(); temp++) {
						if (vertices[temp].vbd.Ids[3] > 32) {
							__debugbreak();
						}
					}
				}

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

		void SkeletalModel::BoneTransform(float _totalTime, std::vector<glm::mat4>& _matrices)
		{

			_matrices.resize(numberOfBones);

			glm::mat4 rootNodeMatrix(1.0f);

			// TODO: Check if valid scene, before accessing Animations here.
			const auto ticks_per_second = scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f;
			auto time_in_ticks = _totalTime * ticks_per_second;
			auto animation_time = fmod(time_in_ticks, scene->mAnimations[0]->mDuration);

			ReadNodeHierarchyAnimation(animation_time, scene->mRootNode, rootNodeMatrix);

			// For now, set them to Identity.
			for (auto i = 0; i < numberOfBones; i++) {
				_matrices[i] = boneInfoData[i].finalTransformation;
			}

		}

		void SkeletalModel::ReadNodeHierarchyAnimation(float _animationTime, const aiNode* _node,
			const glm::mat4& _parentTransform)
		{

			std::string node_name = _node->mName.data;

			const aiAnimation * p_animation = scene->mAnimations[0];

			glm::mat4 node_transformation(1.0f);

			convert_aimatrix_to_glm(node_transformation, _node->mTransformation);

			const aiNodeAnim * node_anim = FindNodeAnim(p_animation, node_name);

			if (node_anim) {

				glm::mat4 transformation_matrix(1.0f);

				//// Interpolate stuff..
				//aiVector3D scaling;
				//CalcInterpolatedScaling(scaling, _animationTime, node_anim);
				//transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scaling.x, scaling.y, scaling.z));

				//aiQuaternion rotation;
				//CalcInterpolatedRotation(rotation, _animationTime, node_anim);

				//glm::mat4 rotataion_matrix;
				//convert_aimatrix_to_glm(rotataion_matrix, rotation.GetMatrix());

				//transformation_matrix *= rotataion_matrix;

				aiVector3D translation;
				CalcInterpolatedPosition(translation, _animationTime, node_anim);

				transformation_matrix = glm::translate(transformation_matrix, glm::vec3(translation.x, translation.y, translation.z));

				node_transformation = transformation_matrix;

			}

			glm::mat4 global_transformation = node_transformation * _parentTransform;

			if (boneMapping.find(node_name) != boneMapping.end()) {
				// Update the Global Transformation.
				auto bone_index = boneMapping[node_name];

				//boneInfoData[bone_index].finalTransformation = globalInverseTransform * global_transformation * boneInfoData[bone_index].boneOffset;
				//boneInfoData[bone_index].finalTransformation = boneInfoData[bone_index].boneOffset * global_transformation * globalInverseTransform;
				boneInfoData[bone_index].finalTransformation = glm::mat4(1.0f);
			}

			for (auto i = 0; i < _node->mNumChildren; i++) {
				ReadNodeHierarchyAnimation(_animationTime, _node->mChildren[i], global_transformation);
			}

		}

		const aiNodeAnim * SkeletalModel::FindNodeAnim(const aiAnimation * _animation, const std::string& _nodeName)
		{

			for (auto i = 0 ; i < _animation->mNumChannels; i++)
			{
				const aiNodeAnim * node_anim = _animation->mChannels[i];

				if (std::string(node_anim->mNodeName.data) == _nodeName) {
					return node_anim;
				}
			}

			return nullptr;
		}
	}
}