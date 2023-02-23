#pragma once

#include "Graphics.h"
#include "TextureManager.h"
#include "MeshManager.h"

class Engine
{
public:
	static void Create();
	static void Release();
	static Engine* GetInstance();

	Graphics* GetGraphics();
	TextureManager* GetTextureManager();
	MeshManager* GetMeshManager();

	MaterialPtr CreateMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	MaterialPtr CreateMaterial(const MaterialPtr& material);
	void SetMaterial(const MaterialPtr& material);

	SpritePtr CreateSprite(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	SpritePtr CreateSprite(const SpritePtr& sprite);
	void SetSprite(const SpritePtr& sprite);

	void GetVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size);

private:
	Engine();
	~Engine();

	static Engine* m_engine;
	Graphics* m_graphics = nullptr;
	TextureManager* m_texture_manager = nullptr;
	MeshManager* m_mesh_manager = nullptr;
	unsigned char m_vertex_mesh_layout_byte_code[1024] = {};
	size_t m_vertex_mesh_layout_size = 0;
};

