#pragma once

#include "QEMDebug.h"
#include "assimp_helper.h"
#include <vector>
#include <map>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/TriConnectivity.hh>


class QEM
{
  public:
	enum SimplificationMode
	{
		Position,
		PositionNormal,
		PositionNormalUV
	};

	struct MeshTraits : public OpenMesh::DefaultTraits
	{
		using Point      = OpenMesh::Vec3d;
		using Normal     = OpenMesh::Vec3d;
		using TexCoord2D = OpenMesh::Vec2d;
		VertexAttributes(OpenMesh::Attributes::Status | OpenMesh::Attributes::Normal | OpenMesh::Attributes::TexCoord2D);
		FaceAttributes(OpenMesh::Attributes::Status | OpenMesh::Attributes::Normal);
		EdgeAttributes(OpenMesh::Attributes::Status);
		HalfedgeAttributes(OpenMesh::Attributes::Status);
	};

	using QEMMesh = OpenMesh::TriMesh_ArrayKernelT<MeshTraits>;

  private:
	QEMMesh heMesh;

  public:
	inline void ImportMesh(const Mesh &mesh)
	{
		std::vector<QEMMesh::VertexHandle> vhs;
		std::map<std::array<double, 3>, QEMMesh::VertexHandle> mp;

		for (int i = 0; i < mesh.V.rows(); i++)
		{
			std::array<double, 3> pt{mesh.V(i, 0), mesh.V(i, 1), mesh.V(i, 2)};
			QEMMesh::VertexHandle vh;
			if (mp.find(pt) != mp.end())
				vh = mp[pt];
			else {
				vh = heMesh.add_vertex(QEMMesh::Point(mesh.V(i, 0), mesh.V(i, 1), mesh.V(i, 2)));
				heMesh.set_normal(vh, QEMMesh::Normal(mesh.N(i, 0), mesh.N(i, 1), mesh.N(i, 2)));
				heMesh.set_texcoord2D(vh, QEMMesh::TexCoord2D(mesh.UV(i, 0), mesh.UV(i, 1)));
				mp[pt] = vh;
			}
			vhs.push_back(vh);
		}

		for (int i = 0; i < mesh.F.rows(); i++)
		{
			heMesh.add_face(vhs[mesh.F(i, 0)], vhs[mesh.F(i, 1)], vhs[mesh.F(i, 2)]);
		}
	}

	inline Mesh ExportMesh()
	{
		Mesh outputMesh;
		outputMesh.V.resize(heMesh.n_vertices(), 3);
		outputMesh.F.resize(heMesh.n_faces(), 3);
		outputMesh.N.resize(heMesh.n_vertices(), 3);
		outputMesh.UV.resize(heMesh.n_vertices(), 2);

		for (int i = 0; i < heMesh.n_vertices(); i++)
		{
			auto vh            = heMesh.vertex_handle(i);
			outputMesh.V(i, 0) = heMesh.point(vh)[0];
			outputMesh.V(i, 1) = heMesh.point(vh)[1];
			outputMesh.V(i, 2) = heMesh.point(vh)[2];
			outputMesh.N(i, 0) = heMesh.normal(vh)[0];
			outputMesh.N(i, 1) = heMesh.normal(vh)[1];
			outputMesh.N(i, 2) = heMesh.normal(vh)[2];
			outputMesh.UV(i, 0) = heMesh.texcoord2D(vh)[0];
			outputMesh.UV(i, 1) = heMesh.texcoord2D(vh)[1];
		}

		for (int i = 0; i < heMesh.n_faces(); i++)
		{
			auto fh  = heMesh.face_handle(i);
			auto it  = heMesh.fv_cwbegin(fh);
			int  idx = 0;
			while (it != heMesh.fv_cwend(fh))
			{
				assert(idx < 3);
				outputMesh.F(i, idx) = it->idx();
				idx++;
				it++;
			}
		}

		return outputMesh;
	}

	inline bool DoSimplification(SimplificationMode mode, int targetFaceCount)
	{
		QEM_DEBUG("DoSimplification(mode=%d, targetFaceCount=%d)", mode, targetFaceCount);
		// TODO: implement this

		return true;
	}
};