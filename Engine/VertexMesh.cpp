#include "pch.h"
#include "VertexMesh.h"
#include "Vector2.h"
#include "Vector3.h"

VertexMesh::VertexMesh(Vector3 position, Vector2 texcoord, Vector3 normal)
	: m_position(position)
	, m_texcoord(texcoord)
	, m_normal(normal)
{
}

VertexMesh::VertexMesh(const VertexMesh& vertex_mesh)
	: m_position(vertex_mesh.m_position)
	, m_texcoord(vertex_mesh.m_texcoord)
	, m_normal(vertex_mesh.m_normal)
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
