#pragma once
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include "Shader.h"

#include "VertexBuffer.h"
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

	std::cout << glGetString(GL_VERSION) << std::endl;

	{

		float positions[6] = {
			-0.5f,	-0.5f,
			0.0f,	 0.5f,
			0.5f,	-0.5f
		};

		unsigned int vao;

		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		VertexBuffer vb(positions, 6 * sizeof(float));

		// This should always be after the Bind buffer data for some reason..
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0));

		//olab::Shader test = olab::Shader("Assets/Shaders/Basic.vert", "Assets/Shaders/Basic.frag");
		olab::Shader test = olab::Shader("Assets/Shaders/Basic.shader");

		GLCall(glUseProgram(test.shaderId));

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			GLCall(glUseProgram(test.shaderId));
			GLCall(glBindVertexArray(vao));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb.GetRendererId()));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}

	glfwTerminate();
	return 0;

}
