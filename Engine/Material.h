#pragma once

enum class CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK
};

class Material
{
public:
	Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	Material(const MaterialPtr& material);
	~Material();

	VertexShaderPtr GetVertexShader();
	PixelShaderPtr GetPixelShader();
	ConstantBufferPtr GetConstantBuffer();
	TexturePtr GetTexture();
	size_t GetTextureSize();

	void AddTexture(const TexturePtr& texture);
	void RemoveTexture(unsigned int index);

	void SetData(void* data, unsigned int size);

	CULL_MODE GetCullMode();
	void SetCullMode(CULL_MODE cull_mode);

private:
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	ConstantBufferPtr m_constant_buffer;
	std::vector<TexturePtr> m_vec_textures;
	CULL_MODE m_cull_mode = CULL_MODE::CULL_MODE_BACK;
};

