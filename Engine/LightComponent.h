#pragma once

#include "Component.h"

class LightComponent : public Component
{
public:
	LightComponent();
	virtual ~LightComponent();

	Vector4 GetColor() const;
	void SetColor(const Vector4& color);

protected:
	virtual void OnCreate() override;

private:
	Vector4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
};

