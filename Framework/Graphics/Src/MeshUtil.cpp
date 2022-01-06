#include "Precompiled.h"
#include "MeshUtil.h"

void Sage::Graphics::MeshUtil::ComputeNormal(Mesh& mesh)
{
	std::vector<Math::Vector3> normals;
	normals.resize(mesh.vertices.size());
	for (size_t i = 0; i < mesh.indices.size(); i += 3)
	{
		const auto ia = mesh.indices[i];
		const auto ib = mesh.indices[i + 1];
		const auto ic = mesh.indices[i + 2];
		auto& va = mesh.vertices[ia];
		auto& vb = mesh.vertices[ib];
		auto& vc = mesh.vertices[ic];
		const auto ab = Math::Normalize(vb.position - va.position);
		const auto ac = Math::Normalize(vc.position - va.position);
		const auto normal = Math::Normalize(Math::Cross(ab, ac));
		normals[ia] += normal;
		normals[ib] += normal;
		normals[ic] += normal;
	}

	for (size_t i = 0; i < mesh.vertices.size(); ++i)
		mesh.vertices[i].normal = Math::Normalize(normals[i]);
}
