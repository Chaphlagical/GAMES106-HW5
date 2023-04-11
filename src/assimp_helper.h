#pragma once

#include <Eigen/Dense>
#include <assimp/scene.h>

#include <string>
#include <vector>

struct aiScene;

struct Texture
{
	Eigen::Matrix<uint8_t, -1, -1> R;
	Eigen::Matrix<uint8_t, -1, -1> G;
	Eigen::Matrix<uint8_t, -1, -1> B;
};

struct Mesh
{
	Eigen::MatrixXd V;
	Eigen::MatrixXd N;
	Eigen::MatrixXd UV;
	Eigen::MatrixXi F;

	int32_t texture = -1;

	// For rendering
	bool is_visible = true;
};

struct Instance
{
	std::vector<size_t> submeshes;
	Eigen::Matrix4d     transform;

	// For rendering
	bool is_visible = true;
};

struct Scene
{
	std::string filename;

	const aiScene *assimp_scene;

	std::vector<Mesh>     meshes;
	std::vector<Instance> instances;
	std::vector<Texture>  textures;

	size_t num_mesh       = 0;
	size_t num_instance   = 0;
	size_t num_total_mesh = 0;
};

class AssimpHelper
{
  public:
	static Scene Load(const char *filename, uint32_t flags = 0);
	static void  Save(const char *filename, const Scene &scene, const char *flags);
};