#pragma once

#include "Rect.h"

class App
{
public:
	App();
	virtual ~App();

	void Run();
	void Quit();

	World* GetWorld() const;
	Display* GetDisplay() const;
	Graphics* GetGraphics() const;

protected:
	virtual void OnCreate();
	virtual void OnUpdate(float delta_time);
	virtual void OnQuit();

private:
	void Core();
	void Resize(const Rect& size);

	bool m_running = true;

	std::unique_ptr<Input> m_input = nullptr;
	std::unique_ptr<Timer> m_timer = nullptr;
	std::unique_ptr<World> m_world = nullptr;
	std::unique_ptr<Graphics> m_graphics = nullptr;
	std::unique_ptr<Display> m_display = nullptr;
	std::unique_ptr<ResourceManager> m_resource_manager = nullptr;

	MeshPtr m_mesh = nullptr;
	TexturePtr m_texture = nullptr;
	MaterialPtr m_material = nullptr;

	friend Display;
};
