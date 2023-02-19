#include "pch.h"
#include "Engine.h"

Engine* Engine::m_engine = nullptr;

void Engine::Create()
{
    assert(!m_engine);
    if (m_engine)
        throw std::exception("Engine already created");

    Engine::m_engine = new Engine();
}

void Engine::Release()
{
    if (!m_engine)
        return;

    delete m_engine;
    m_engine = nullptr;
}

Engine* Engine::GetInstance()
{
    return m_engine;
}

Graphics* Engine::GetGraphics()
{
    return m_graphics;
}

TextureManager* Engine::GetTextureManager()
{
    return m_texture_manager;
}

MeshManager* Engine::GetMeshManager()
{
    return m_mesh_manager;
}

void Engine::GetMeshVertexLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
    *byte_code = m_mesh_vertex_layout_byte_code;
    *size = m_mesh_vertex_layout_size;
}

Engine::Engine()
{
    try
    {
        m_graphics = new Graphics();
    }
    catch (...)
    {
        assert(m_graphics);
        throw std::exception("Graphics not created successfully");
    }

    try
    {
        m_texture_manager = new TextureManager();
    }
    catch (...)
    {
        assert(m_texture_manager);
        throw std::exception("TextureManager not created successfully");
    }

    try
    {
        m_mesh_manager = new MeshManager();
    }
    catch (...)
    {
        assert(m_mesh_manager);
        throw std::exception("MeshManager not created successfully");
    }

    void* shader_byte_code = nullptr;
    size_t shader_byte_size = 0;
    m_graphics->CompileVertexShader(L"..\\..\\Assets\\Shaders\\MeshVertexLayoutShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
    assert(shader_byte_code);
    assert(shader_byte_size);
    ::memcpy(m_mesh_vertex_layout_byte_code, shader_byte_code, shader_byte_size);
    m_mesh_vertex_layout_size = shader_byte_size;
    m_graphics->ReleaseCompiledShader();
}

Engine::~Engine()
{
    if (m_graphics)
    {
        delete m_graphics;
        m_graphics = nullptr;
    }

    if (m_texture_manager)
    {
        delete m_texture_manager;
        m_texture_manager = nullptr;
    }

    if (m_mesh_manager)
    {
        delete m_mesh_manager;
        m_mesh_manager = nullptr;
    }
}
