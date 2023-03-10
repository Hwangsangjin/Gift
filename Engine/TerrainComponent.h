#pragma once

#include "Component.h"
#include "Vector3.h"

class TerrainComponent : public Component
{
public:
	TerrainComponent();
	virtual ~TerrainComponent();

	const TexturePtr& GetHeightMap() const;
	void SetHeightMap(const TexturePtr& height_map);

	const TexturePtr& GetGroundMap() const;
	void SetGroundMap(const TexturePtr& ground_map);

	const TexturePtr& GetCliffMap() const;
	void SetCliffMap(const TexturePtr& cliff_map);

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
	void GenerateTerrainMesh();

	Vector3 m_size = { 512, 100, 512 };

	TexturePtr m_height_map = nullptr;
	TexturePtr m_ground_map = nullptr;
	TexturePtr m_cliff_map = nullptr;

	VertexBufferPtr m_vertex_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;
	ConstantBufferPtr m_constant_buffer = nullptr;

	VertexShaderPtr m_vertex_shader = nullptr;
	PixelShaderPtr m_pixel_shader = nullptr;
};

