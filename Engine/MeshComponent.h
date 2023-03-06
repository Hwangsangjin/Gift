#pragma once

#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent();
	virtual ~MeshComponent();

	const MeshPtr& GetMesh();
	void SetMesh(const MeshPtr& mesh);

	void AddMaterial(const MaterialPtr& material);
	void RemoveMaterial(UINT index);

	const std::vector<MaterialPtr>& GetMaterials();

//protected:
	virtual void OnCreate() override;

private:
	MeshPtr m_mesh;
	std::vector<MaterialPtr> m_materials;
};

