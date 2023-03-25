#include "assimp_helper.h"

#include <assimp/DefaultLogger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>

#include <igl/edge_flaps.h>

#include <iostream>
#include <filesystem>

static Assimp::Importer s_importer;
static const aiScene* s_scene;

inline  Eigen::Matrix4d convert_matrix(const aiMatrix4x4& transform)
{
	Eigen::Matrix4d ret;
	ret(0, 0) = transform.a1; ret(0, 1) = transform.b1; ret(0, 2) = transform.c1; ret(0, 3) = transform.d1;
	ret(1, 0) = transform.a2; ret(1, 1) = transform.b2; ret(1, 2) = transform.c2; ret(1, 3) = transform.d2;
	ret(2, 0) = transform.a3; ret(2, 1) = transform.b3; ret(2, 2) = transform.c3; ret(2, 3) = transform.d3;
	ret(3, 0) = transform.a4; ret(3, 1) = transform.b4; ret(3, 2) = transform.c4; ret(3, 3) = transform.d4;
	return ret;
}

Scene AssimpHelper::Load(const char* filename, uint32_t flags)
{
	Scene scene;

	scene.num_instance = 0;
	scene.num_mesh = 0;
	scene.num_total_mesh = 0;

	if (s_scene = s_importer.ReadFile(filename, flags | aiProcess_JoinIdenticalVertices))
	{
		scene.filename = filename;
		scene.assimp_scene = s_scene;

		// Handle submesh
		scene.meshes.resize(scene.assimp_scene->mNumMeshes);
		for (uint32_t i = 0; i < scene.assimp_scene->mNumMeshes; i++)
		{
			
			scene.num_mesh++;

			aiMesh* assimp_mesh = scene.assimp_scene->mMeshes[i];

			const uint32_t num_vertices = assimp_mesh->mNumVertices;
			const uint32_t num_faces = assimp_mesh->mNumFaces;

			scene.meshes[i].V.resize(num_vertices, 3);
			scene.meshes[i].N.resize(num_vertices, 3);
			scene.meshes[i].UV.resize(num_vertices, 2);
			scene.meshes[i].F.resize(num_faces, 3);

			for (uint32_t j = 0; j < num_vertices; j++)
			{
				scene.meshes[i].V(j, 0) = assimp_mesh->mVertices[j].x;
				scene.meshes[i].V(j, 1) = assimp_mesh->mVertices[j].y;
				scene.meshes[i].V(j, 2) = assimp_mesh->mVertices[j].z;

				scene.meshes[i].N(j, 0) = assimp_mesh->mNormals ? assimp_mesh->mNormals[j].x : 0.f;
				scene.meshes[i].N(j, 1) = assimp_mesh->mNormals ? assimp_mesh->mNormals[j].y : 0.f;
				scene.meshes[i].N(j, 2) = assimp_mesh->mNormals ? assimp_mesh->mNormals[j].z : 0.f;

				scene.meshes[i].UV(j, 0) = assimp_mesh->mTextureCoords[0] ? assimp_mesh->mTextureCoords[0][j].x : 0.f;
				scene.meshes[i].UV(j, 1) = assimp_mesh->mTextureCoords[0] ? assimp_mesh->mTextureCoords[0][j].y : 0.f;
			}

			for (uint32_t j = 0; j < num_faces; j++)
			{
				// TODO
				assert(assimp_mesh->mFaces[j].mNumIndices == 3);
				for (uint32_t k = 0; k < 3; k++)
				{
					scene.meshes[i].F(j, k) = assimp_mesh->mFaces[j].mIndices[k];
				}
			}

			igl::edge_flaps(scene.meshes[i].F, scene.meshes[i].E, scene.meshes[i].EMAP, scene.meshes[i].EF, scene.meshes[i].EI);
		}

		// Handle hierarchy
		scene.instances.clear();
		Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
		aiNode* root = scene.assimp_scene->mRootNode;
		std::function<void(aiNode*)> traverse_node = [&](aiNode* node) {
			scene.num_instance++;
			transform *= convert_matrix(node->mTransformation);
			Instance instance;
			instance.transform = transform;
			for (uint32_t i = 0; i < node->mNumMeshes; i++)
			{
				instance.submeshes.push_back(node->mMeshes[i]);
				scene.num_total_mesh++;
			}
			scene.instances.emplace_back(std::move(instance));
			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				traverse_node(node->mChildren[i]);
			}
		};

		traverse_node(root);

	}

	return scene;
}

void AssimpHelper::Save(const char* filename, const Scene& scene, const char* flags)
{
	for (uint32_t i = 0; i < scene.assimp_scene->mNumMeshes; i++)
	{
		aiMesh* assimp_mesh = scene.assimp_scene->mMeshes[i];

		const uint32_t num_vertices = scene.meshes[i].V.rows();
		const uint32_t num_faces = scene.meshes[i].F.rows();

		if (assimp_mesh->mNumVertices != num_vertices)
		{
			assimp_mesh->mNumVertices = num_vertices;
			if (assimp_mesh->mVertices)
			{
				delete[] assimp_mesh->mVertices;
				assimp_mesh->mVertices = nullptr;
			}
			assimp_mesh->mVertices = new aiVector3D[num_vertices];
		}

		if (assimp_mesh->mNumFaces != num_faces)
		{
			assimp_mesh->mNumFaces = num_faces;
			if (assimp_mesh->mNumFaces)
			{
				delete[] assimp_mesh->mFaces;
				assimp_mesh->mFaces = nullptr;
			}
			assimp_mesh->mFaces = new aiFace[num_faces];
		}

		for (uint32_t j = 0; j < num_vertices; j++)
		{
			assimp_mesh->mVertices[j].x = scene.meshes[i].V(j, 0);
			assimp_mesh->mVertices[j].y = scene.meshes[i].V(j, 1);
			assimp_mesh->mVertices[j].z = scene.meshes[i].V(j, 2);
		}

		for (uint32_t j = 0; j < num_faces; j++)
		{
			// TODO
			assert(assimp_mesh->mFaces[j].mNumIndices == 3);
			for (uint32_t k = 0; k < 3; k++)
			{
				assimp_mesh->mFaces[j].mIndices[k] = scene.meshes[i].F(j, k);
			}
		}
	}
	Assimp::Exporter exporter;
	exporter.Export(scene.assimp_scene, flags, filename);
}
