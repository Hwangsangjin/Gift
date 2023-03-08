#pragma once

#include "Component.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent();
	virtual ~SpriteComponent();

	void SetTexture(const TexturePtr& texture);

protected:
	virtual void OnCreate() override;

private:
	TexturePtr m_texture = nullptr;

	friend RenderSystem;
};

