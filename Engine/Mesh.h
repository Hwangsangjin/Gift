#pragma once

#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh : public Resource
{
public:
	Mesh(const wchar_t* full_path);
	virtual ~Mesh() override;

	const VertexBufferPtr& GetVertexBuffer();
	const IndexBufferPtr& GetIndexBuffer();

private:
	VertexBufferPtr m_vertex_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;
};

