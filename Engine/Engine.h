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

class Engine
{
public:
	Engine(App* app);

	void Update(const ObjectData& mesh_data);

	Graphics* GetGraphics();

private:
	std::unique_ptr<Graphics> m_graphics = nullptr;
	App* m_app = nullptr;
};

