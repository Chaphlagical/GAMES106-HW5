#include "mesh_reduction.h"

#include <Eigen/Sparse>
#include <igl/collapse_edge.h>
#include <igl/min_heap.h>
#include <igl/parallel_for.h>
#include <igl/vertex_triangle_adjacency.h>
#include <igl/edge_flaps.h>
#include <igl/shortest_edge_and_midpoint.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <memory_resource>
#include <set>
#include <thread>

Mesh MeshReduction::Reduction(const Mesh &mesh_, float ratio)
{
	return Mesh();
}

Mesh MeshReduction::ReductionWithAppearancePresentation(const Mesh &mesh, float ratio)
{
	return Mesh();
}

Mesh MeshReduction::ReductionWithUVMap(const Mesh &mesh, float ratio)
{
	return Mesh();
}
