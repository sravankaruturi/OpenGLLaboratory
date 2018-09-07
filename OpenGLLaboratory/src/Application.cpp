#include "Renderer.h"
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#include "Texture.h"

#include "../external_files/ImGUI/imgui.h"
#include "../external_files/ImGUI/imgui_impl_glfw.h"
#include "../external_files/ImGUI/imgui_impl_opengl3.h"

#include "Concepts/ConceptClearColour.h"	
#include "Concepts/ConceptModelViewProjection.h"
#include "Concepts/ConceptModelLoading.h"
#include "Concepts/ConceptStencilTest.h"
#include "Concepts/ConceptMultipleViewports.h"
#include "Concepts/ConceptSkeletalMesh.h"

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

int width =		1600;
int height =	900;

void window_resize(GLFWwindow * _window, int _width, int _height);

int main(int _argc, char* _argv[])
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow * window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, window_resize);

	/* Load Glad */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	const olab::Renderer renderer;

	/* ImGui setup */
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();

	olab::concepts::Concept * current_concept = nullptr;
	olab::concepts::ConceptsMenu * concepts_menu = new olab::concepts::ConceptsMenu(current_concept);

	concepts_menu->RegisterConcept<olab::concepts::ConceptClearColour>("Clear Colour");
	concepts_menu->RegisterConcept<olab::concepts::ConceptModelViewProjection>("Model View Projection Matrices");
	concepts_menu->RegisterConcept<olab::concepts::ConceptModelLoading>("Model Loading");
	concepts_menu->RegisterConcept<olab::concepts::ConceptMultipleViewports>("Multiple Viewports");
	concepts_menu->RegisterConcept<olab::concepts::ConceptStencilTest>("Stencil Testing");
	concepts_menu->RegisterConcept<olab::concepts::ConceptSkeletalMesh>("Skeletal Mesh Animation");
	
	// We Draw the Current Test and we want to start with the Menu usually.
	// current_concept = concepts_menu;

	const auto concepts_size = concepts_menu->concepts.size();
	current_concept = concepts_menu->concepts[concepts_size - 1].second();

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
				current_concept->OnRender(renderer);
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

void window_resize(GLFWwindow * _window, int _width, int _height) {
	width = _width;
	height = _height;
	//glViewport(0, 0, _width, _height);
}