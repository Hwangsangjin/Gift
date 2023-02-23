#include "pch.h"
#include "Engine.h"
#include "DeviceContext.h"
#include "Material.h"
#include "Sprite.h"

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

MaterialPtr Engine::CreateMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
    return std::make_shared<Material>(vertex_shader_path, pixel_shader_path);
}

MaterialPtr Engine::CreateMaterial(const MaterialPtr& material)
{
    return std::make_shared<Material>(material);
}

void Engine::SetMaterial(const MaterialPtr& material)
{
    // 래스터라이저 상태 설정
    m_graphics->SetCullMode(material->GetCullMode() == Material::CullMode::Back);
    m_graphics->SetFillMode(material->GetFillMode() == Material::FillMode::Solid);

    // 상수 버퍼 설정
    m_graphics->GetDeviceContext()->SetConstantBuffer(material->GetVertexShader(), material->GetConstantBuffer());
    m_graphics->GetDeviceContext()->SetConstantBuffer(material->GetPixelShader(), material->GetConstantBuffer());

    // 셰이더 설정
    m_graphics->GetDeviceContext()->SetVertexShader(material->GetVertexShader());
    m_graphics->GetDeviceContext()->SetPixelShader(material->GetPixelShader());
    auto texture = material->GetTexture();
    m_graphics->GetDeviceContext()->SetTexture(material->GetPixelShader(), &texture, material->GetTextureSize());
}

SpritePtr Engine::CreateSprite(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
    return std::make_shared<Sprite>(vertex_shader_path, pixel_shader_path);
}

SpritePtr Engine::CreateSprite(const SpritePtr& sprite)
{
    return std::make_shared<Sprite>(sprite);
}

void Engine::SetSprite(const SpritePtr& sprite)
{
    // 래스터라이저 상태 설정
    m_graphics->SetCullMode(sprite->GetCullMode() == Sprite::CullMode::Back);
    m_graphics->SetFillMode(sprite->GetFillMode() == Sprite::FillMode::Solid);

    // 상수 버퍼 설정
    m_graphics->GetDeviceContext()->SetConstantBuffer(sprite->GetVertexShader(), sprite->GetConstantBuffer());
    m_graphics->GetDeviceContext()->SetConstantBuffer(sprite->GetPixelShader(), sprite->GetConstantBuffer());

    // 셰이더 설정
    m_graphics->GetDeviceContext()->SetVertexShader(sprite->GetVertexShader());
    m_graphics->GetDeviceContext()->SetPixelShader(sprite->GetPixelShader());
    auto texture = sprite->GetTexture();
    m_graphics->GetDeviceContext()->SetTexture(sprite->GetPixelShader(), &texture, sprite->GetTextureSize());
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
