#pragma once
#include "Concept.h"

#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"

namespace olab {

	namespace concepts {

		class ConceptModelViewProjection : public Concept
		{

		private:

			const float positions[180] = {
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

			olab::Shader mvpShader;

			olab::Texture texture;

			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			glm::mat4 modelMatrix;

			glm::vec3 cameraPosition;
			glm::vec3 cameraFront;
			glm::vec3 worldUp;
			glm::mat4 viewMatrix;

			float fieldOfView;
			glm::mat4 projectionMatrix;

		public:

			explicit ConceptModelViewProjection();
			~ConceptModelViewProjection();

			void OnUpdate(float _deltaTime);
			void OnRender(const Renderer& _renderer);
			void OnImGuiRender();

		};


	}
}
