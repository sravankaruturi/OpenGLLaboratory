#include "ConceptSkeletalMesh.h"

#include <filesystem>
#include <iostream>

#include <glm/gtc/matrix_transform.inl>
#include "../../../external_files/ImGUI/imgui.h"

#include <GLFW/glfw3.h>

#include "../../Configuration.h"

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
	}
}