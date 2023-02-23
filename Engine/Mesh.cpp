#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "VertexMesh.h"
#include "Vector2.h"
#include "Vector3.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyObjLoader/tiny_obj_loader.h"

#include <locale>
#include <codecvt>
#include <filesystem>

Mesh::Mesh(const wchar_t* full_path)
	: Resource(full_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string error;

	auto input_file = std::filesystem::path(full_path).string();

	std::string mtl_dir = input_file.substr(0, input_file.find_last_of("\\/"));

	bool result = tinyobj::LoadObj(&attribs, &shapes, &materials, &warning, &error, input_file.c_str(), mtl_dir.c_str());
	if (!error.empty())
		throw std::exception("Mesh not created successfully");
	if (!result)
		throw std::exception("Mesh not created successfully");

	std::vector<VertexMesh> mesh_vertices;
	std::vector<UINT> mesh_indices;

	size_t vertex_buffer_size = 0;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		vertex_buffer_size += shapes[s].mesh.indices.size();
	}

	mesh_vertices.reserve(vertex_buffer_size);
	mesh_indices.reserve(vertex_buffer_size);

	m_material_slots.resize(materials.size());

	size_t index_global_offset = 0;

	for (size_t m = 0; m < materials.size(); m++)
	{
		m_material_slots[m].start_index = index_global_offset;
		m_material_slots[m].material_id = m;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				if (shapes[s].mesh.material_ids[f] != m)
					continue;

				unsigned char num_face_vertices = shapes[s].mesh.num_face_vertices[f];

				for (unsigned char v = 0; v < num_face_vertices; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

					tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
					tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];

					VertexMesh mesh_vertex(Vector3(vx, vy, vz), Vector2(tx, ty), Vector3(nx, ny, nz));
					mesh_vertices.push_back(mesh_vertex);
					mesh_indices.push_back((unsigned int)index_global_offset + v);
				}

				index_offset += num_face_vertices;
				index_global_offset += num_face_vertices;
			}
		}

		m_material_slots[m].index_size = index_global_offset - m_material_slots[m].start_index;
	}

	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Engine::GetInstance()->GetVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);

	m_vertex_buffer = Engine::GetInstance()->GetGraphics()->CreateVertexBuffer(&mesh_vertices[0], sizeof(VertexMesh), static_cast<UINT>(mesh_vertices.size()), shader_byte_code, static_cast<UINT>(shader_byte_size));
	assert(m_vertex_buffer);

	m_index_buffer = Engine::GetInstance()->GetGraphics()->CreateIndexBuffer(&mesh_indices[0], static_cast<UINT>(mesh_indices.size()));
	assert(m_index_buffer);
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::GetVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::GetIndexBuffer()
{
	return m_index_buffer;
}

const MaterialSlot& Mesh::GetMaterialSlot(unsigned int slot)
{
	if (slot >= m_material_slots.size())
		return MaterialSlot();

	return m_material_slots[slot];
}

size_t Mesh::GetMaterialSlotSize() const
{
	return m_material_slots.size();
}
