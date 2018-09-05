#pragma once
#include "../Shader.h"
#include "../Texture.h"
#include "../IndexBuffer.h"
#include "../VertexBuffer.h"
#include "../VertexArray.h"
#include "../Shader.h"
#include "../VertexBufferLayout.h"
#include <assimp/material.h>
#include "../Concepts/Concept.h"

#include <map>

#define NUM_BONES_PER_VERTEX	4

struct aiMesh;
struct aiScene;
struct aiNode;

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

		// We can have a shader each for each mesh. But we are not doint that for now, for the sake of simplicity.
		struct SkeletalMesh {

			VertexBuffer * vb;
			VertexArray * va;
			IndexBuffer * ib;
			std::vector<Texture *> textures;
			//Shader * shader;

			// Add any bone Data that is relevant.

		};

		class SkeletalModel {

			std::string filename;
			std::vector<SkeletalMesh> meshes;

			std::vector<VertexBoneData> vertexBoneData;
			std::map<std::string, unsigned int> boneMapping; // Maps a bone to its index in BoneInfo

			std::vector<BoneInfo> boneInfoData;

			bool showSkeleton = false;

			unsigned int numberOfVertices;
			unsigned int numberOfIndices;
			unsigned int numberOfMeshes;

		public:

			Shader * shader;

			explicit SkeletalModel(const std::string &filename);
			~SkeletalModel();

			void LoadModel(const std::string& _filename);

			// Render
			void Render(Shader _shader, const Renderer& _renderer);

			// Update
			void Update(float _deltatime);

			void SetShowSkeleton(bool _showSkeleton);

			void ProcessModel();

			

			const std::vector<Texture *> LoadMaterialTextures(aiMaterial * _material, aiTextureType _textureType, std::string _param3);
		};

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

			SkeletalModel * model;

		public:

			ConceptSkeletalMesh();
			~ConceptSkeletalMesh();

			void OnUpdate(float _deltaTime);
			void OnRender(const Renderer& _renderer);
			void OnImGuiRender();

		};

	}

}
