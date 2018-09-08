#pragma once

#include "SkeletalModel.h"

#include <assimp/material.h>
#include "../../Concepts/Concept.h"

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

			olab::Shader * skinningShader;

			float fieldOfView;
			glm::mat4 projectionMatrix;

			SkeletalModel * model;

			/* ImGUi Toggles */

			bool showBoneWindow = false;
			bool showBoneInfoWindow = false;
			bool showBoneDataPerMesh = false;
			SkeletalMesh * selected_mesh = nullptr;

			/*Imgui Toggles End*/

			std::vector<VertexBoneData> boneDataForMesh;

		public:

			ConceptSkeletalMesh();
			~ConceptSkeletalMesh();

			void OnUpdate(float _deltaTime);
			void OnRender(const Renderer& _renderer);
			void OnImGuiRender();

		};

	}

}
