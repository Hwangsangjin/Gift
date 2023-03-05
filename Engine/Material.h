#pragma once

#include "Resource.h"

class Material : public Resource
{
public:
	enum class CullMode
	{
		Front,
		Back
	};

	enum class FillMode
	{
		Wireframe,
		Solid
	};

	Material(const wchar_t* file_path, ResourceManager* resource_manager);
	Material(const MaterialPtr& material, ResourceManager* resource_manager);
	virtual ~Material() override;

	VertexShaderPtr GetVertexShader();
	PixelShaderPtr GetPixelShader();
	ConstantBufferPtr GetConstantBuffer();

	Texture2DPtr& GetTexture();
	size_t GetTextureSize() const;

	void AddTexture(const TexturePtr& texture);
	void RemoveTexture(UINT index);

	void SetData(void* data, UINT size);

	CullMode GetCullMode();
	void SetCullMode(CullMode cull_mode);
	FillMode GetFillMode();
	void SetFillMode(FillMode fill_mode);

private:
	VertexShaderPtr m_vertex_shader = nullptr;
	PixelShaderPtr m_pixel_shader = nullptr;
	ConstantBufferPtr m_constant_buffer = nullptr;

	std::vector<Texture2DPtr> m_textures = {};

	CullMode m_cull_mode = CullMode::Back;
	FillMode m_fill_mode = FillMode::Solid;
};

