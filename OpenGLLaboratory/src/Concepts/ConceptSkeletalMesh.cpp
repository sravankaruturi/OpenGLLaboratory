#include "ConceptSkeletalMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <iostream>

#include "../IndexBuffer.h"
#include "../VertexBuffer.h"
#include "../VertexArray.h"
#include "../Shader.h"
#include <glm/gtc/matrix_transform.inl>

namespace olab {
	namespace concepts {

		ConceptSkeletalMesh::ConceptSkeletalMesh()
			: skinningShader("Assets/Shaders/Concept_skinning.shader")
		{
			
		}

		ConceptSkeletalMesh::~ConceptSkeletalMesh()
		{

		}

		void ConceptSkeletalMesh::OnUpdate(float _deltaTime)
		{

		}

		void ConceptSkeletalMesh::OnRender(const Renderer& _renderer)
		{

		}

		void ConceptSkeletalMesh::OnImGuiRender()
		{

		}

		void ConceptSkeletalMesh::LoadMesh(std::string _objectpath)
		{

		}

	}
}