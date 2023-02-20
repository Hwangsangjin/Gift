#pragma once

#include "Vector2.h"
#include "Vector3.h"

class VertexMesh
{
public:
	VertexMesh() = default;
	VertexMesh(Vector3 position, Vector2 texcoord, Vector3 normal);
	VertexMesh(const VertexMesh& vertex_mesh);

	Vector3 GetPosition() const;
	Vector2 GetTexcoord() const;
	Vector3 GetNormal() const;

private:
	Vector3 m_position = {};
	Vector2 m_texcoord = {};
	Vector3 m_normal = {};
};

