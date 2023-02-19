#include "pch.h"
#include "MeshManager.h"
#include "Mesh.h"

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

Resource* MeshManager::CreateResourceFromFileConcrete(const wchar_t* file_path)
{
    Mesh* mesh = nullptr;
    try
    {
        mesh = new Mesh(file_path);
    }
    catch (...)
    {
        assert(mesh);
        throw std::exception("Mesh not created successfully");
    }

    return mesh;
}
