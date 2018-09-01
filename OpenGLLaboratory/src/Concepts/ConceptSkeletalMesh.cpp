#include "ConceptSkeletalMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <iostream>

#include "../IndexBuffer.h"
#include "../VertexBuffer.h"
#include "../VertexArray.h"
#include "../Shader.h"
#include <glm/gtc/matrix_transform.inl>
#include "../../external_files/ImGUI/imgui.h"

#include <GLFW/glfw3.h>

namespace olab {
	namespace concepts {

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

			cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
			cameraFront = glm::vec3(.0f, 0.0f, -1.0f);
			viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, worldUp);

			projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.f, 100.f);

			std::string path = "Z:/IGMProfile/Desktop/Projects/OpenGLLaboratory/OpenGLLaboratory/Assets/Models/boblamp/boblampclean.md5mesh";

			ConceptModelLoading::LoadMesh(path);

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

			ConceptModelLoading::OnUpdate(_deltaTime);
		}

		void ConceptSkeletalMesh::OnRender(const Renderer& _renderer)
		{

			Shader * using_shader = meshes[0].shader;

			using_shader->use();
			using_shader->setInt("u_Texture", 0);
			using_shader->setMat4("u_ModelMatrix", modelMatrix);
			using_shader->setMat4("u_ViewMatrix", viewMatrix);
			using_shader->setMat4("u_ProjectionMatrix", projectionMatrix);

			ConceptModelLoading::OnRender(_renderer);
		}

		void ConceptSkeletalMesh::OnImGuiRender()
		{
			ConceptModelLoading::OnImGuiRender();

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

	}
}