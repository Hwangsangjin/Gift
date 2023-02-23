#pragma once

#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct MaterialSlot
{
	size_t start_index = 0;
	size_t index_size = 0;
	size_t material_id = 0;
};

class Mesh : public Resource
{
public:
	Mesh(const wchar_t* full_path);
	virtual ~Mesh() override;

	const VertexBufferPtr& GetVertexBuffer();
	const IndexBufferPtr& GetIndexBuffer();

	const MaterialSlot& GetMaterialSlot(unsigned int slot);
	size_t GetMaterialSlotSize() const;

private:
	VertexBufferPtr m_vertex_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;

	std::vector<MaterialSlot> m_material_slots;
};

