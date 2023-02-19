#pragma once

#include "GameObject.h"

class Plane : public GameObject
{
public:
	Plane();
	virtual ~Plane() override;

	virtual void Update();
	virtual void Render();

private:
	TexturePtr m_texture = nullptr;
};

