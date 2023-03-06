#pragma once

class Graphics
{
public:
	Graphics(App* app);
	~Graphics();
	void Update();

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	Renderer* GetRenderer() const;

private:
	std::unique_ptr<Renderer> m_renderer = nullptr;
	App* m_app = nullptr;

	std::set<MeshComponent*> m_meshes;
	std::set<CameraComponent*> m_cameras;
};

