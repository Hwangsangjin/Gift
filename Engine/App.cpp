#include "pch.h"
#include "App.h"
#include "Input.h"
#include "Timer.h"
#include "World.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Display.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

App::App()
{
    m_input = std::make_unique<Input>();
    m_timer = std::make_unique<Timer>();
    m_world = std::make_unique<World>();
    m_graphics = std::make_unique<Graphics>(this);
    m_display = std::make_unique<Display>(this);
    m_resource_manager = std::make_unique<ResourceManager>(this);

    m_mesh =  m_resource_manager->CreateResourceFromFile<Mesh>(L"..\\..\\Assets\\Meshes\\house.obj");
    m_texture = m_resource_manager->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\wood.jpg");
    m_material = m_resource_manager->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\DefaultShader.hlsl");
    m_material->AddTexture(m_texture);

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

        Core();
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

Graphics* App::GetGraphics() const
{
    return m_graphics.get();
}

Display* App::GetDisplay() const
{
    return m_display.get();
}

World* App::GetWorld() const
{
    return m_world.get();
}

void App::Core()
{
    m_input->Update();
    if (m_input->IsKeyDown(Key::Escape))
    {
        m_input->LockCursor(false);
    }
    
    m_timer->Update();
    OnUpdate(m_timer->GetDeltaTime());
    m_world->Update(m_timer->GetDeltaTime());

    m_graphics->Update({ m_mesh, m_material });
}

void App::Resize(const Rect& size)
{
    m_input->SetLockArea(m_display->GetClientSize());
    Core();
}