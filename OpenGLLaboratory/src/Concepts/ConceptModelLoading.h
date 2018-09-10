﻿#pragma once
#include "Concept.h"
#include "../VertexArray.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"
#include <assimp/material.h>

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

namespace olab
{
	namespace concepts
	{

		class ConceptModelLoading : public Concept
		{

		protected:

			struct Mesh
			{
				VertexBuffer * vb;
				VertexArray * va;
				IndexBuffer * ib;
				std::vector<Texture *> textures;
				Shader * shader;
			};

			std::vector<ConceptModelLoading::Mesh> meshes;

			void ProcessNode(aiNode *node, const aiScene *scene);
			ConceptModelLoading::Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
			std::vector<Texture*> LoadMaterialTextures(aiMaterial *_mat, aiTextureType _type, std::string _typeName);

			std::string directory;

		public:
			ConceptModelLoading();
			~ConceptModelLoading();

			void LoadMesh(std::string _path);

			virtual void OnRender(const Renderer& _renderer) override;
			virtual void OnImGuiRender() override;

		};
	}
}
