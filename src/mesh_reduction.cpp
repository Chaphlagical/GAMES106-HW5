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

#include "UHEMesh/HeMesh.h"

class QEM
{
public:
	class V;
	class E;
	using TraitsVEP = Ubpa::HEMeshTraits_EmptyPH<V, E>;

	class V : public Ubpa::TVertex<TraitsVEP>
	{
	public:
		V(Eigen::Vector3d pos, Eigen::Vector3d normal, Eigen::Vector2d uv) :
		  pos(pos), normal(normal), uv(uv)
		{}

		V() :
		    pos(Eigen::Vector3d::Zero())
		{}
		Eigen::Vector3d pos;
		Eigen::Vector3d normal;
		Eigen::Vector2d uv;
	};

	class E : public Ubpa::TEdge<TraitsVEP>
	{
		// TODO: add edge cost
	};

	using HEMesh = Ubpa::HEMesh<TraitsVEP>;

private:
	HEMesh heMesh;

public:
	inline void ImportMesh(const Mesh& mesh)
	{
		heMesh.Clear();

		for (size_t i = 0; i < mesh.V.rows(); i++)
		{
			heMesh.AddVertex(mesh.V.row(i), mesh.N.row(i), mesh.UV.row(i));
		}

		for (size_t fIndex = 0; fIndex < mesh.F.rows(); fIndex++)
		{
			std::array<HEMesh::H *, 3> heLoop;
			for (size_t i = 0; i < 3; i++)
			{
				size_t next = (i + 1) % 3;
				assert(mesh.F(fIndex, i) != mesh.F(fIndex, next));
				auto *u  = heMesh.Vertices()[mesh.F(fIndex, i)];
				auto *v  = heMesh.Vertices()[mesh.F(fIndex, next)];
				auto *he = u->HalfEdgeTo(v);
				if (!he)
				{
					he = heMesh.AddEdge(u, v)->HalfEdge();
				}
					
				heLoop[i] = he;
			}
			auto *p = heMesh.AddPolygon(heLoop);
			assert(p != nullptr);
		}
	}

	inline Mesh ExportMesh()
	{
		Mesh mesh;
		
		mesh.F.resize(heMesh.Polygons().size(), 3);
		mesh.V.resize(heMesh.Vertices().size(), 3);
		mesh.N.resize(heMesh.Vertices().size(), 3);
		mesh.UV.resize(heMesh.Vertices().size(), 2);

		for (size_t i = 0; i < heMesh.Vertices().size(); i++)
		{
			mesh.V.row(i) = heMesh.Vertices()[i]->pos;
			mesh.N.row(i) = heMesh.Vertices()[i]->normal;
			mesh.UV.row(i) = heMesh.Vertices()[i]->uv;
		}

		for (size_t i = 0; i < heMesh.Polygons().size(); i++)
		{
			auto tri = heMesh.Polygons()[i];
			auto triHeIt = tri->AdjHalfEdges().begin();

			for (int j = 0; j < 3; j++)
			{
				mesh.F(i, j) = heMesh.Index(triHeIt->Origin());
				triHeIt++;
			}

			assert(triHeIt == tri->AdjHalfEdges().end());
		}

		return mesh;
	}
};

Mesh MeshReduction::Reduction(const Mesh &mesh, float ratio)
{
	QEM qem;
	qem.ImportMesh(mesh);
	// TODO: implement me

	Mesh outputMesh = qem.ExportMesh();
	return outputMesh;
}

Mesh MeshReduction::ReductionWithAppearancePresentation(const Mesh &mesh, float ratio)
{
	QEM qem;
	qem.ImportMesh(mesh);
	// TODO: implement me

	Mesh outputMesh = qem.ExportMesh();
	return outputMesh;
}

Mesh MeshReduction::ReductionWithUVMap(const Mesh &mesh, float ratio)
{
	QEM qem;
	qem.ImportMesh(mesh);
	// TODO: implement me

	Mesh outputMesh = qem.ExportMesh();
	return outputMesh;
}
