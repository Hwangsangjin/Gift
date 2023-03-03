#include "pch.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "VertexMesh.h"

MeshManager::MeshManager()
    : ResourceManager()
{
}

MeshManager::~MeshManager()
{
}

MeshPtr MeshManager::CreateMeshFromFile(const wchar_t* file_path)
{
    return std::static_pointer_cast<Mesh>(CreateResourceFromFile(file_path));
}

MeshPtr MeshManager::CreateMesh(VertexMesh* vertex_mesh_data, UINT vertex_size, UINT* index_data, UINT index_size, MaterialSlot* material_slot, UINT material_slot_size)
{
    Mesh* mesh = new Mesh(vertex_mesh_data, vertex_size, index_data, index_size, material_slot, material_slot_size);
    assert(mesh);
    MeshPtr mesh_ptr(mesh);
    assert(mesh_ptr);
    return mesh_ptr;
}

Resource* MeshManager::CreateResourceFromFileConcrete(const wchar_t* file_path)
{
    Mesh* mesh = new Mesh(file_path);
    assert(mesh);
    return mesh;
}
