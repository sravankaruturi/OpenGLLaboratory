#include "Renderer.h"
#include <GLFW/glfw3.h>
#include "Shader.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

#include <iostream>

int main(int _argc, char* _argv[])
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow * window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

	olab::Renderer renderer;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{

		float positions[6] = {
			-0.5f,	-0.5f,
			0.0f,	 0.5f,
			0.5f,	-0.5f
		};

		const unsigned int indices[3] = {
			0, 1, 2
		};


		olab::VertexArray va;
		olab::VertexBuffer vb(positions, 6 * sizeof(float));
		olab::VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		olab::IndexBuffer ib(indices, 3);

		//olab::Shader test = olab::Shader("Assets/Shaders/Basic.vert", "Assets/Shaders/Basic.frag");
		olab::Shader test = olab::Shader("Assets/Shaders/Basic.shader");



		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			olab::Renderer::Clear();

			test.use();
			test.setVec3("u_Colour", glm::vec3(glm::cos(glfwGetTime()), 0.2f, 0.3f));

			renderer.Draw(va, ib, test);
			
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}

	glfwTerminate();
	return 0;

}
