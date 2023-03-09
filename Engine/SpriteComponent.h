#pragma once

#include "Component.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent();
	virtual ~SpriteComponent();

	const MeshPtr& GetMesh();
	void SetMesh(const MeshPtr& mesh);

	void AddMaterial(const MaterialPtr& material);
	void RemoveMaterial(UINT index);

	const std::vector<MaterialPtr>& GetMaterials();

protected:
	virtual void OnCreate() override;

private:
	MeshPtr m_mesh;
	std::vector<MaterialPtr> m_materials;
};

