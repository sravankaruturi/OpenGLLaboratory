#pragma once
#include "ConceptModelLoading.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../VertexBufferLayout.h"
#include <assimp/material.h>

namespace olab {

	namespace concepts {

		class ConceptSkeletalMesh : public Concept {

			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			glm::mat4 modelMatrix;

			glm::vec3 cameraPosition;
			glm::vec3 cameraFront;
			glm::vec3 worldUp;
			glm::mat4 viewMatrix;

			olab::Shader skinningShader;

			float fieldOfView;
			glm::mat4 projectionMatrix;

		public:

			ConceptSkeletalMesh();
			~ConceptSkeletalMesh();

			void OnUpdate(float _deltaTime);
			void OnRender(const Renderer& _renderer);
			void OnImGuiRender();

		private:

			void LoadMesh(std::string _objectpath);

		};

	}

}