#pragma once

#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Vector2;
class Vector3;

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

	const MaterialSlot& GetMaterialSlot(UINT slot);
	size_t GetMaterialSlotSize() const;

private:
	void ComputeTangent
	(
		const Vector3 v0, const Vector3 v1, const Vector3 v2,
		const Vector2 t0, const Vector2 t1, const Vector2 t2,
		Vector3& tangent, Vector3 binormal
	);

	VertexBufferPtr m_vertex_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;

	std::vector<MaterialSlot> m_material_slots;
};

