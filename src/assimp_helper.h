#pragma once

#include <assimp/scene.h>
#include <Eigen/Dense>

#include <vector>
#include <string>

struct aiScene;

struct Mesh
{
	Eigen::MatrixXd V;
	Eigen::MatrixXd N;
	Eigen::MatrixXd UV;
	Eigen::MatrixXi F;
	Eigen::MatrixXi E, EF, EI;
	Eigen::VectorXi EMAP;

	// For rendering
	bool is_visible = true;
};

struct Instance
{
	std::vector<size_t> submeshes;
	Eigen::Matrix4d transform;

	// For rendering
	bool is_visible = true;
};

struct Scene
{
	std::string filename;

	const aiScene* assimp_scene;

	std::vector<Mesh> meshes;
	std::vector<Instance> instances;

	size_t num_mesh = 0;
	size_t num_instance = 0;
	size_t num_total_mesh = 0;
};

class AssimpHelper
{
public:
	static Scene Load(const char* filename, uint32_t flags = 0);
	static void Save(const char* filename, const Scene& scene, const char* flags);
};