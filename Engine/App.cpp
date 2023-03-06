#include "pch.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "World.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Display.h"
#include "ResourceManager.h"

App::App()
{
    m_input = std::make_unique<Input>();
    m_timer = std::make_unique<Timer>();
    m_graphics = std::make_unique<Graphics>(this);
    m_display = std::make_unique<Display>(this);
    m_world = std::make_unique<World>(this);
    m_resource_manager = std::make_unique<ResourceManager>(this);

    m_input->SetLockArea(m_display->GetClientSize());
}

App::~App()
{
}

void App::Run()
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

void App::OnCreate()
{
}

void App::OnUpdate(float delta_time)
{
}

void App::OnQuit()
{
}

void App::Quit()
{
    m_running = false;
}

Input* App::GetInput() const
{
    return m_input.get();
}

Timer* App::GetTimer() const
{
    return m_timer.get();
}

World* App::GetWorld() const
{
    return m_world.get();
}

Display* App::GetDisplay() const
{
    return m_display.get();
}

Graphics* App::GetGraphics() const
{
    return m_graphics.get();
}

ResourceManager* App::GetResourceManager() const
{
    return m_resource_manager.get();
}

void App::OnCore()
{
    m_input->Update();
    if (m_input->IsKeyDown(Key::Escape))
    {
        m_input->LockCursor(false);
    }
    
    m_timer->Update();
    OnUpdate(m_timer->GetDeltaTime());
    m_world->Update(m_timer->GetDeltaTime());

    m_graphics->Update();
}

void App::Resize(const Rect& size)
{
    m_input->SetLockArea(m_display->GetClientSize());
    OnCore();
}