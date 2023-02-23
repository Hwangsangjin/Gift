#pragma once

class Sprite
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

	Sprite(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	Sprite(const SpritePtr& sprite);
	~Sprite();

	VertexBufferPtr GetVertexBuffer();
	VertexShaderPtr GetVertexShader();
	PixelShaderPtr GetPixelShader();
	ConstantBufferPtr GetConstantBuffer();
	IndexBufferPtr GetIndexBuffer();
	TexturePtr GetTexture();
	size_t GetTextureSize();

	void AddTexture(const TexturePtr& texture);
	void RemoveTexture(unsigned int index);

	void SetData(void* data, unsigned int size);

	CullMode GetCullMode();
	void SetCullMode(CullMode cull_mode);
	FillMode GetFillMode();
	void SetFillMode(FillMode fill_mode);

private:
	VertexBufferPtr m_vertex_buffer = nullptr;
	VertexShaderPtr m_vertex_shader = nullptr;
	PixelShaderPtr m_pixel_shader = nullptr;
	ConstantBufferPtr m_constant_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;

	std::vector<TexturePtr> m_vec_textures;

	CullMode m_cull_mode = CullMode::Back;
	FillMode m_fill_mode = FillMode::Solid;
};

