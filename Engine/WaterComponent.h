#pragma once

#include "Component.h"
#include "Vector3.h"

class WaterComponent : public Component
{
public:
	WaterComponent();
	virtual ~WaterComponent();

	const TexturePtr& GetWaveHeightMap() const;
	void SetWaveHeightMap(const TexturePtr& wave_height_map);

	void SetSize(const Vector3& size);
	Vector3 GetSize() const;

	const VertexBufferPtr& GetVertexBuffer() const;
	const IndexBufferPtr& GetIndexBuffer() const;
	const ConstantBufferPtr& GetConstantBuffer() const;
	const VertexShaderPtr& GetVertexShader() const;
	const PixelShaderPtr& GetPixelShader() const;

	void SetConstantData(void* constant_data, UINT constant_size);

protected:
	virtual void OnCreate() override;

private:
	void GenerateMesh();

	Vector3 m_size = { 2048.0f, 8.0f, 2048.0f };

	TexturePtr m_wave_height_map = nullptr;

	VertexBufferPtr m_vertex_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;
	ConstantBufferPtr m_constant_buffer = nullptr;

	VertexShaderPtr m_vertex_shader = nullptr;
	PixelShaderPtr m_pixel_shader = nullptr;
};

