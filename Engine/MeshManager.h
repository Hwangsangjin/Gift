#pragma once

#include "ResourceManager.h"

class VertexMesh;
class MaterialSlot;

class MeshManager : public ResourceManager
{
public:
	MeshManager();
	virtual ~MeshManager() override;

	MeshPtr CreateMeshFromFile(const wchar_t* file_path);
	MeshPtr CreateMesh(VertexMesh* vertex_mesh_data, UINT vertex_size, UINT* index_data, UINT index_size, MaterialSlot* material_slot, UINT material_slot_size);

protected:
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* file_path) override;
};

