#include "pch.h"
#include "Engine.h"
#include "Timer.h"
#include "World.h"
#include "Display.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "RenderSystem.h"
#include "ResourceManager.h"

Engine::Engine()
{
    m_input_system = std::make_unique<InputSystem>(this);
    m_sound_system = std::make_unique<SoundSystem>(this);
    m_render_system = std::make_unique<RenderSystem>(this);
    m_display = std::make_unique<Display>(this);
    m_resource_manager = std::make_unique<ResourceManager>(this);
    m_world = std::make_unique<World>(this);
    m_timer = std::make_unique<Timer>(this);

    m_input_system->SetLockArea(m_display->GetClientSize());
}

Engine::~Engine()
{
}

void Engine::Run()
{
    OnCreate();

    while (m_running)
    {
        MSG msg = {};
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                m_running = false;
                continue;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        OnCore();
    }

    OnQuit();
}

void Engine::OnCreate()
{
}

void Engine::OnUpdate(float delta_time)
{
}

void Engine::OnQuit()
{
}

void Engine::Quit()
{
    m_running = false;
}

InputSystem* Engine::GetInputSystem() const
{
    return m_input_system.get();
}

SoundSystem* Engine::GetSoundSystem() const
{
    return m_sound_system.get();
}

RenderSystem* Engine::GetRenderSystem() const
{
    return m_render_system.get();
}

Display* Engine::GetDisplay() const
{
    return m_display.get();
}

ResourceManager* Engine::GetResourceManager() const
{
    return m_resource_manager.get();
}

World* Engine::GetWorld() const
{
    return m_world.get();
}

Timer* Engine::GetTimer() const
{
    return m_timer.get();
}

void Engine::OnCore()
{
    m_input_system->Update();

    m_timer->Update();
    OnUpdate(m_timer->GetDeltaTime());
    m_world->Update(m_timer->GetDeltaTime());

    m_render_system->Update();
    m_display->Update();
}

void Engine::Resize(const Rect& size)
{
    m_input_system->SetLockArea(m_display->GetClientSize());
    OnCore();
}