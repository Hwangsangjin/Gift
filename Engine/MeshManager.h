#pragma once

#include "ResourceManager.h"

class MeshManager : public ResourceManager
{
public:
	MeshManager();
	virtual ~MeshManager() override;

	MeshPtr CreateMeshFromFile(const wchar_t* file_path);

protected:
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* file_path) override;
};

