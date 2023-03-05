#include "pch.h"
#include "App.h"
#include "Input.h"
#include "Engine.h"
#include "Display.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

App::App()
{
    m_input = std::make_unique<Input>();
    m_engine = std::make_unique<Engine>(this);
    m_display = std::make_unique<Display>(this);
    m_resource_manager = std::make_unique<ResourceManager>(this);

    m_mesh =  m_resource_manager->CreateResourceFromFile<Mesh>(L"..\\..\\Assets\\Meshes\\house.obj");
    m_texture = m_resource_manager->CreateResourceFromFile<Texture>(L"..\\..\\Assets\\Textures\\wood.jpg");
    m_material = m_resource_manager->CreateResourceFromFile<Material>(L"..\\..\\Assets\\Shaders\\DefaultShader.hlsl");
    m_material->AddTexture(m_texture);

    m_input->SetLockArea(m_display->GetClientSize());
    m_input->LockCursor(true);
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

void App::OnUpdate()
{
}

void App::OnQuit()
{
}

void App::Quit()
{
    m_running = false;
}

void App::Resize(const Rect& size)
{
    m_input->SetLockArea(m_display->GetClientSize());
    OnCore();
}

Engine* App::GetEngine() const
{
    return m_engine.get();
}

Display* App::GetDisplay() const
{
    return m_display.get();
}

void App::OnCore()
{
    m_input->Update();
    if (m_input->IsKeyDown(Key::Escape))
    {
        m_input->LockCursor(false);
    }

    m_engine->Update({ m_mesh, m_material });
}
