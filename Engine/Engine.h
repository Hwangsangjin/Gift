#pragma once

#include "Rect.h"

class Engine
{
public:
	Engine();
	virtual ~Engine();

	void Run();
	void Quit();

	InputSystem* GetInputSystem() const;
	SoundSystem* GetSoundSystem() const;
	RenderSystem* GetRenderSystem() const;
	Display* GetDisplay() const;
	ResourceManager* GetResourceManager() const;
	World* GetWorld() const;
	Timer* GetTimer() const;

protected:
	virtual void OnCreate();
	virtual void OnUpdate(float delta_time);
	virtual void OnQuit();

private:
	void OnCore();
	void Resize(const Rect& size);

	bool m_running = true;

	std::unique_ptr<InputSystem> m_input_system = nullptr;
	std::unique_ptr<SoundSystem> m_sound_system = nullptr;
	std::unique_ptr<RenderSystem> m_render_system = nullptr;
	std::unique_ptr<Display> m_display = nullptr;
	std::unique_ptr<ResourceManager> m_resource_manager = nullptr;
	std::unique_ptr<World> m_world = nullptr;
	std::unique_ptr<Timer> m_timer = nullptr;

	friend Display;
};

