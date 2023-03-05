#pragma once

struct ObjectData
{
	MeshPtr mesh;
	MaterialPtr material;
};

__declspec(align(16))
struct ConstantData
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
};

class Graphics
{
public:
	Graphics(App* app);

	void Update(const ObjectData& mesh_data);

	Renderer* GetRenderer();

private:
	std::unique_ptr<Renderer> m_renderer = nullptr;
	App* m_app = nullptr;
};

