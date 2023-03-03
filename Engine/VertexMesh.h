#pragma once

#include "Vector2.h"
#include "Vector3.h"

class VertexMesh
{
public:
	VertexMesh() = default;
	VertexMesh(const Vector3& position, const Vector2& texcoord, const Vector3& normal, const Vector3& tangent, const Vector3& binormal);
	VertexMesh(const VertexMesh& vertex_mesh);

private:
	Vector3 m_position = {};
	Vector2 m_texcoord = {};
	Vector3 m_normal = {};
	Vector3 m_tangent = {};
	Vector3 m_binormal = {};
};

