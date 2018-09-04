#pragma once
#include "ConceptModelLoading.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../VertexBufferLayout.h"
#include <assimp/material.h>

#include <map>

#define NUM_BONES_PER_VERTEX	4

namespace olab {

	namespace concepts {

		struct BoneInfo {
			glm::mat4 boneOffset;
			glm::mat4 finalTransformation;
		};

		// The Bone Data for a specific vertex. It would ass the Indices of the Bones from the Bone Info Structure, and the Weight of each bone.
		struct VertexBoneData {
			unsigned int Ids[NUM_BONES_PER_VERTEX];
			float weights[NUM_BONES_PER_VERTEX];

			VertexBoneData() {
				Reset();
			}

			void Reset() {
				memset(Ids, 0, sizeof(Ids));
				memset(weights, 0, sizeof(weights));
			}

			void AddBoneData(unsigned int _boneID, float _weight);

		};

		class ConceptSkeletalMesh : public ConceptModelLoading {

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

			std::vector<VertexBoneData> vertexBoneData;
			std::map<std::string, unsigned int> boneMapping; // Maps a bone to its index in BoneInfo

			std::vector<BoneInfo> boneInfoData;

		public:

			ConceptSkeletalMesh();
			~ConceptSkeletalMesh();

			void OnUpdate(float _deltaTime);
			void OnRender(const Renderer& _renderer);
			void OnImGuiRender();

			void LoadMesh(std::string _path);
			void ProcessNode(aiNode * _node, const aiScene * _scene);

			// This should be a different type of mesh since we do not have the structure ready for the bones in the previous mesh data.
			Mesh ProcessMesh(aiMesh *_mesh, const aiScene *_scene);

		};

	}

}