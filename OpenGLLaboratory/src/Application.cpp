#include "Renderer.h"
#include <GLFW/glfw3.h>
#include "Shader.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

#include <iostream>
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../external_files/ImGUI/imgui.h"
#include "../external_files/ImGUI/imgui_impl_glfw.h"
#include "../external_files/ImGUI/imgui_impl_opengl3.h"

#include "Concepts/ConceptClearColour.h"	

int main(int _argc, char* _argv[])
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow * window = glfwCreateWindow(800, 450, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Load Glad */
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to Init Glew" << std::endl;
		return -1;
	}

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	olab::Renderer renderer;

	/* ImGui setup */
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();

	olab::concepts::Concept * current_concept = nullptr;
	olab::concepts::ConceptsMenu * concepts_menu = new olab::concepts::ConceptsMenu(current_concept);

	concepts_menu->RegisterConcept<olab::concepts::ConceptClearColour>("Clear Colour");

	// We Draw the Current Test and we want to start with the Menu usually.
	current_concept = concepts_menu;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			olab::Renderer::Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (current_concept) {
				current_concept->OnUpdate(0.0f);
				current_concept->OnRender();
				ImGui::Begin("Concepts");
				if (current_concept != concepts_menu && ImGui::Button("<--")) {
					delete current_concept;
					current_concept = concepts_menu;
				}
				current_concept->OnImGuiRender();
				ImGui::End();
			}
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		// If we quit while not at the main menu, we need to delete both the test and the menu.
		if (concepts_menu != current_concept) {
			delete current_concept;
		}
		delete concepts_menu;

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;

}
