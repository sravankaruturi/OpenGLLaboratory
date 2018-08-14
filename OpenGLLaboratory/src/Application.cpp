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


	std::cout << glGetString(GL_VERSION) << std::endl;

	{

		float positions[] = {
			-0.5f,	-0.5f,	0.0f,	0.0f,
			0.5f,	-0.5f,	1.0f,	0.0f,
			0.5f,	0.5f,	1.0f,	1.0f,
			-0.5f,	0.5f,	0.0f,	1.0f
		};

		const unsigned int indices[] = {
			0, 1, 2, 
			2, 3, 0
		};


		olab::VertexArray va;
		olab::VertexBuffer vb(positions, sizeof(positions));
		olab::VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		olab::IndexBuffer ib(indices, 6);

		glm::vec3 translation(1.0f, 0.0f, 0.0f);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, translation);

		const glm::mat4 projection_matrix = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);

		//olab::Shader test = olab::Shader("Assets/Shaders/Basic.vert", "Assets/Shaders/Basic.frag");
		olab::Shader test = olab::Shader("Assets/Shaders/Basic.shader");

		olab::Texture texture("Assets/Textures/iris_texture.png");
		texture.Bind();

		test.use();
		test.setInt("u_Texture", 0);
		test.setMat4("u_ProjectionMatrix", projection_matrix);


		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			olab::Renderer::Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{

				ImGui::SliderFloat3("float", glm::value_ptr(translation), -5.0f, 5.0f);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			model = glm::translate(glm::mat4(1.0f), translation);

			test.use();
			test.setMat4("u_ModelMatrix", model);
			//test.setVec3("u_Colour", glm::vec3(glm::cos(glfwGetTime()), 0.2f, 0.3f));
			//test.setInt("u_Texture", 0);
			renderer.Draw(va, ib, test);
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;

}
