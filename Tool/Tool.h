#pragma once

#include "Engine.h"

class Tool : public Engine
{
public:
	Tool();
	virtual ~Tool() override;

protected:
	virtual void OnCreate() override;
	virtual void OnUpdate(float delta_time) override;

private:
	bool m_locked = true;
	float m_rotation = 0.0f;

	Entity* m_entity = nullptr;
	Entity* m_shine = nullptr;

	TexturePtr m_shine_texture[10];

	UINT m_apply_index = 0;
	float m_life_time = 0.0f;
	float m_render_time = 0.0f;
	std::vector<UINT> m_indices;

	MeshComponent* m_mesh_component = nullptr;
	
	SpriteComponent* m_sprite_component = nullptr;

	AudioComponent* m_audio_component = nullptr;

	MaterialPtr m_plane_material = nullptr;
	MaterialPtr m_shine_material = nullptr;
	MaterialPtr m_sphere_material = nullptr;
};

