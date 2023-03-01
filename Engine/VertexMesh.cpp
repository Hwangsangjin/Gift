#include "pch.h"
#include "VertexMesh.h"
#include "Vector2.h"
#include "Vector3.h"

VertexMesh::VertexMesh(const Vector3& position, const Vector2& texcoord, const Vector3& normal, const Vector3& tangent, const Vector3& binormal)
	: m_position(position)
	, m_texcoord(texcoord)
	, m_normal(normal)
	, m_tangent(tangent)
	, m_binormal(binormal)
{
}

VertexMesh::VertexMesh(const VertexMesh& vertex_mesh)
	: m_position(vertex_mesh.m_position)
	, m_texcoord(vertex_mesh.m_texcoord)
	, m_normal(vertex_mesh.m_normal)
	, m_tangent(vertex_mesh.m_tangent)
	, m_binormal(vertex_mesh.m_binormal)
{
}

Vector3 VertexMesh::GetPosition() const
{
	return m_position;
}

Vector2 VertexMesh::GetTexcoord() const
{
	return m_texcoord;
}

Vector3 VertexMesh::GetNormal() const
{
	return m_normal;
}
