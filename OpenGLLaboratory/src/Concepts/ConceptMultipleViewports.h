#pragma once
#include "Concept.h"

#include "../VertexArray.h"
#include "../VertexBufferLayout.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Renderer.h"

struct GLFWwindow;

namespace olab {

	namespace concepts {

		class ConceptMultipleViewports : public Concept
		{

		private:

			GLFWwindow * window;

			const float positions[108] = {
				-0.5f, -0.5f, -0.5f, 
				0.5f, -0.5f, -0.5f,  
				0.5f,  0.5f, -0.5f,  
				0.5f,  0.5f, -0.5f,  
				-0.5f,  0.5f, -0.5f, 
				-0.5f, -0.5f, -0.5f, 

				-0.5f, -0.5f,  0.5f, 
				0.5f, -0.5f,  0.5f,  
				0.5f,  0.5f,  0.5f,  
				0.5f,  0.5f,  0.5f,  
				-0.5f,  0.5f,  0.5f, 
				-0.5f, -0.5f,  0.5f, 

				-0.5f,  0.5f,  0.5f, 
				-0.5f,  0.5f, -0.5f, 
				-0.5f, -0.5f, -0.5f, 
				-0.5f, -0.5f, -0.5f, 
				-0.5f, -0.5f,  0.5f, 
				-0.5f,  0.5f,  0.5f, 

				0.5f,  0.5f,  0.5f,  
				0.5f,  0.5f, -0.5f,  
				0.5f, -0.5f, -0.5f,  
				0.5f, -0.5f, -0.5f,  
				0.5f, -0.5f,  0.5f,  
				0.5f,  0.5f,  0.5f,  

				-0.5f, -0.5f, -0.5f, 
				0.5f, -0.5f, -0.5f,  
				0.5f, -0.5f,  0.5f,  
				0.5f, -0.5f,  0.5f,  
				-0.5f, -0.5f,  0.5f, 
				-0.5f, -0.5f, -0.5f, 

				-0.5f,  0.5f, -0.5f, 
				0.5f,  0.5f, -0.5f,  
				0.5f,  0.5f,  0.5f,  
				0.5f,  0.5f,  0.5f,  
				-0.5f,  0.5f,  0.5f, 
				-0.5f,  0.5f, -0.5f
			};

			const unsigned int indices[36] = {
				0, 1, 2,
				3, 4, 5,
				6, 7, 8,
				9, 10, 11,
				12, 13, 14,
				15, 16, 17,
				18, 19, 20,
				21, 22, 23,
				24, 25, 26,
				27, 28, 29,
				30, 31, 32,
				33, 34, 35
			};

			olab::VertexArray va;
			olab::VertexBuffer vb;
			olab::VertexBufferLayout layout;
			olab::IndexBuffer ib;

			olab::Shader multipleViewportsShader;

		public:

			ConceptMultipleViewports(GLFWwindow * _window);
			~ConceptMultipleViewports() = default;

			void OnUpdate(float _deltaTime) override;
			void OnRender(const Renderer& _renderer) override;

		};

	}
}
