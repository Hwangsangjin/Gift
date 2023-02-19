#pragma once

#include "Resource.h"

class Texture : public Resource
{
public:
	Texture(const wchar_t* full_path);
	virtual ~Texture() override;

	ID3D11ShaderResourceView* GetShaderResourceView() const;

private:
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_resource_view = nullptr;
};

