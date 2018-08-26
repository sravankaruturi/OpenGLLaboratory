#include "ConceptMultipleViewports.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../Renderer.h"

namespace olab {
	namespace concepts {

		ConceptMultipleViewports::ConceptMultipleViewports()
			: vb(positions, 180 * sizeof(float)),
			ib(indices, sizeof(indices) / sizeof(float)),
			multipleViewportsShader("Assets/Shaders/Concept_multipleViewports.shader")
		{

		}

		void ConceptMultipleViewports::OnUpdate(float _deltaTime)
		{

			glm::mat4 model_matrix[4]{
				glm::mat4(1.0f),
				glm::mat4(1.0f),
				glm::mat4(1.0f),
				glm::mat4(1.0f)
			};

			for (auto i = 0; i < 4; i++) {
				model_matrix[i] = glm::rotate(model_matrix[i], glm::radians(30.0f * i), glm::vec3(0.0f, 0.0f, 1.0f));
			}

			multipleViewportsShader.use();
			const auto loc = multipleViewportsShader.getUniformLocation("u_ModelMatrix");
			GLCall(glUniformMatrix4fv(loc, 4, GL_FALSE, glm::value_ptr(model_matrix[0])));
		}

		void ConceptMultipleViewports::OnRender(const Renderer& _renderer)
		{

			multipleViewportsShader.use();

			_renderer.Draw(va, ib, multipleViewportsShader);

		}
	}
}