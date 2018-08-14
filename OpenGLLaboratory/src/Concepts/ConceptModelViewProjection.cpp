#include "ConceptModelViewProjection.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "../../external_files/ImGUI/imgui.h"

namespace olab {
	namespace concepts {



		ConceptModelViewProjection::ConceptModelViewProjection()
			: vb(positions, sizeof(positions)), 
			ib(indices, sizeof(indices)/sizeof(float)), 
			mvpShader("Assets/Shaders/Concept_mvp.shader"), 
			texture("Assets/Textures/container.jpg"), 
			position(0.0f, 0.0f, 0.0f),
			rotation(0.0f, 0.0f, 0.0f),
			scale(1.0f, 1.0f, 1.0f),
			modelMatrix(glm::mat4(1.0f)),
			worldUp(0.0f, 1.0f, 0.0f),
			fieldOfView(45.0f)
		{

			layout.Push<float>(3);
			layout.Push<float>(2);
			va.AddBuffer(vb, layout);

			cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
			cameraFront = glm::vec3(.0f, 0.0f, -1.0f);
			viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, worldUp);

			projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.f, 100.f);

			texture.Bind();

		}

		ConceptModelViewProjection::~ConceptModelViewProjection()
		{

		}

		void ConceptModelViewProjection::OnUpdate(float _deltaTime)
		{

			rotation.y = glm::sin(glfwGetTime()) * 90.0f;
			rotation.z = glm::cos(glfwGetTime()) * 90.0f;

			modelMatrix = glm::translate(glm::mat4(1.0f), position);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			modelMatrix = glm::scale(modelMatrix, scale);

			viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, worldUp);

			projectionMatrix = glm::perspective(glm::radians(fieldOfView), 16.0f / 9.0f, 0.1f, 100.f);
		}

		void ConceptModelViewProjection::OnRender(const Renderer& _renderer)
		{
			mvpShader.use();
			mvpShader.setInt("u_Texture", 0);
			mvpShader.setMat4("u_ModelMatrix", modelMatrix);
			mvpShader.setMat4("u_ViewMatrix", viewMatrix);
			mvpShader.setMat4("u_ProjectionMatrix", projectionMatrix);

			_renderer.Draw(va, ib, mvpShader);
		}

		void ConceptModelViewProjection::OnImGuiRender()
		{
			ImGui::Text("Model Matrix");
			ImGui::SliderFloat3("Position", glm::value_ptr(position), -5.0f, 5.0f, "%.3f", 2.0f);
			ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), -90.0f, 90.0f, "%.3f", 0.25f);
			ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.2f, 2.0f, "%.3f", 0.25f);
			ImGui::Separator();
			ImGui::Text("View Matrix");
			ImGui::SliderFloat3("Camera Position", glm::value_ptr(cameraPosition), 0.5f, 15.0f, "%.3f", 2.0f);
			ImGui::Separator();
			ImGui::Text("Projection Matrix");
			ImGui::SliderFloat("Field Of View", &fieldOfView, 0.5f, 90.0f, "%.3f", 2.0f);
		}

	}
}
