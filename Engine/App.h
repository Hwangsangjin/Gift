#pragma once

#include "Rect.h"

class App
{
public:
	App();
	virtual ~App();

	void Run();
	void Quit();
	void Resize(const Rect& size);

	Engine* GetEngine() const;
	Display* GetDisplay() const;

protected:
	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnQuit();

private:
	void OnCore();

	bool m_running = true;

	std::unique_ptr<Input> m_input = nullptr;
	std::unique_ptr<Engine> m_engine = nullptr;
	std::unique_ptr<Display> m_display = nullptr;
	std::unique_ptr<ResourceManager> m_resource_manager = nullptr;

	MeshPtr m_mesh = nullptr;
	TexturePtr m_texture = nullptr;
	MaterialPtr m_material = nullptr;
};

