#include "pch.h"
#include "Engine.h"

Engine* Engine::m_engine = nullptr;

void Engine::Create()
{
    assert(!m_engine);
    if (m_engine)
        throw std::exception("Engine already created");

    m_engine = new Engine();
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

void Engine::GetVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
    *byte_code = m_vertex_mesh_layout_byte_code;
    *size = m_vertex_mesh_layout_size;
}

Engine::Engine()
{
    m_graphics = new Graphics();
    assert(m_graphics);

    m_texture_manager = new TextureManager();
    assert(m_texture_manager);

    m_mesh_manager = new MeshManager();
    assert(m_mesh_manager);

    void* shader_byte_code = nullptr;
    size_t shader_byte_size = 0;
    m_graphics->CompileVertexShader(L"..\\..\\Assets\\Shaders\\VertexMeshLayoutShader.hlsl", "main", &shader_byte_code, &shader_byte_size);
    assert(shader_byte_code);
    assert(shader_byte_size);
    ::memcpy(m_vertex_mesh_layout_byte_code, shader_byte_code, shader_byte_size);
    m_vertex_mesh_layout_size = shader_byte_size;
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
