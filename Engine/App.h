#pragma once

#include "Rect.h"

class App
{
public:
	App();
	virtual ~App();

	void Run();
	void Quit();

	Input* GetInput() const;
	Timer* GetTimer() const;
	World* GetWorld() const;
	Display* GetDisplay() const;
	Graphics* GetGraphics() const;
	ResourceManager* GetResourceManager() const;

protected:
	virtual void OnCreate();
	virtual void OnUpdate(float delta_time);
	virtual void OnQuit();

private:
	void OnCore();
	void Resize(const Rect& size);

	bool m_running = true;

	std::unique_ptr<Input> m_input = nullptr;
	std::unique_ptr<Timer> m_timer = nullptr;
	std::unique_ptr<Graphics> m_graphics = nullptr;
	std::unique_ptr<Display> m_display = nullptr;
	std::unique_ptr<ResourceManager> m_resource_manager = nullptr;
	std::unique_ptr<World> m_world = nullptr;

	friend Display;
};

