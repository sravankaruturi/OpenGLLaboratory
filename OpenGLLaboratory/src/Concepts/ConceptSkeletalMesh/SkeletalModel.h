#pragma once

#include "../../VertexBuffer.h"
#include "../../IndexBuffer.h"
#include "../../VertexArray.h"
#include "../../VertexBufferLayout.h"

#include "../../Shader.h"
#include "../../Texture.h"

#include <map>
#include <assimp/anim.h>
#include <assimp/Importer.hpp>
#include <assimp/material.h>

struct aiAnimation;
struct aiMesh;
struct aiScene;
struct aiNode;

#define NUM_BONES_PER_VERTEX	4

namespace olab {
	namespace concepts {

		struct BoneInfo {
			glm::mat4 boneOffset;
			glm::mat4 finalTransformation;
		};

		// The Bone Data for a specific vertex. It would ass the Indices of the Bones from the Bone Info Structure, and the Weight of each bone.
		struct VertexBoneData {

			// The Ids is the bone Ids.
			unsigned int Ids[NUM_BONES_PER_VERTEX] = { 0, 0, 0, 0 };
			float weights[NUM_BONES_PER_VERTEX] = { 0, 0, 0, 0 };

			VertexBoneData() {
				Reset();
			}

			void Reset() {
				memset(Ids, 0, sizeof(Ids));
				memset(weights, 0, sizeof(weights));
			}

			void AddBoneData(unsigned int _boneID, float _weight);

		};

		// This represents the data you send to the buffer regarding one vertex
		struct VertexData {
			float position[3];
			//float normal[3];
			float texCoord[2];
			VertexBoneData vbd;
		};

		// We can have a shader each for each mesh. But we are not doing that for now, for the sake of simplicity.
		struct SkeletalMesh {

			// Vertex Buffer should contain all the data needed to render stuff.

			VertexBuffer * vb;
			VertexArray * va;
			IndexBuffer * ib;
			std::vector<Texture *> textures;

			// Store this for Debug Purposes.
			std::vector<VertexBoneData> vertexBoneData;

		};

		class SkeletalModel {

			std::string filename;
		public:
			std::vector<SkeletalMesh> meshes;


		private:
			bool showSkeleton = false;

			unsigned int numberOfVertices;
			unsigned int numberOfIndices;
			unsigned int numberOfMeshes;
			unsigned int numberOfBones = 0;

			// This Imported needs to be a class variable since We need it to last or the Scene gets empty??
			Assimp::Importer import;

			unsigned int FindScaling(float _animationTime, const aiNodeAnim* _nodeAnim);
			unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
			unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
			void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
			void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
			void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

		public:

			glm::mat4 globalInverseTransform;

			const aiScene * scene;

			std::map<std::string, unsigned int> boneMapping; // Maps a bone to its index in BoneInfo

			std::vector<olab::concepts::BoneInfo> boneInfoData;

			Shader * shader;

			explicit SkeletalModel(const std::string &filename, Shader * _shader);
			~SkeletalModel();

			void LoadModel(const std::string& _filename);

			// Render
			void Render(float _deltaTime, const Renderer& _renderer);

			// Update
			void Update(float _deltatime);

			void SetShowSkeleton(bool _showSkeleton);

			void ProcessModel();

			const std::vector<Texture *> LoadMaterialTextures(aiMaterial * _material, aiTextureType _textureType, std::string _param3);

			// This function transforms and loads the Bone's Final Matrices.
			void BoneTransform(float _totalTime, std::vector<glm::mat4>& _matrices);

			void ReadNodeHierarchyAnimation(float _animationTime, const aiNode * _node, const aiMatrix4x4& _parentTransform);

			const aiNodeAnim * FindNodeAnim(const aiAnimation * _animation, const std::string& _nodeName);

		};

	}
}