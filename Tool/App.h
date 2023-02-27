#pragma once

#include "Window.h"
#include "Engine.h"
#include "Matrix4x4.h"

class Plane;

class App : public Window
{
public:
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnSize() override;
	virtual void OnFocus() override;
	virtual void OnKillFocus() override;
	virtual void OnDestroy() override;

	virtual void OnKeyUp(int key) override;
	virtual void OnKeyDown(int key) override;

	virtual void OnMouseMove(const Point& point) override;

	virtual void OnLeftButtonUp(const Point& point) override;
	virtual void OnLeftButtonDown(const Point& point) override;
	virtual void OnRightButtonUp(const Point& point) override;
	virtual void OnRightButtonDown(const Point& point) override;

	void Update();
	void UpdateThirdPersonCamera();
	void UpdateCamera();
	void UpdateLight();
	void UpdateSkyBox();
	void UpdateModel(Vector3 position, Vector3 rotation, Vector3 scale, const std::vector<MaterialPtr>& materials);
	void UpdateSprite(Vector3 position, const SpritePtr& sprite, float anim_time);
	void UpdateUI(Vector3 position, const SpritePtr& sprite);

	void Render();

	void DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials);
	void DrawSprite(const SpritePtr& sprite);

private:
	SwapChainPtr m_swap_chain = nullptr;

	MeshPtr m_skybox_mesh = nullptr;
	MeshPtr m_plane_mesh = nullptr;

	TexturePtr m_skybox_texture = nullptr;
	TexturePtr m_plane_texture = nullptr;
	TexturePtr m_shine_texture[10];
	TexturePtr m_number_texture[10];

	SpritePtr m_plane_sprite = nullptr;
	SpritePtr m_shine_sprite = nullptr;

	MaterialPtr m_skybox_material = nullptr;
	MaterialPtr m_plane_material = nullptr;
	MaterialPtr m_shine_material = nullptr;

	std::vector<MaterialPtr> m_materials;
	std::vector<UINT> m_indices;

	bool m_play_state = false;
	bool m_fullscreen_state = false;
	bool m_projection_state = false;

	float m_rotation_x = 0.0f;
	float m_rotation_y = 0.0f;
	float m_light_rotation_y = 0.0f;
	float m_light_radius = 500.0f;
	float m_upward = 0.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	float m_delta_mouse_x = 0.0f;
	float m_delta_mouse_y = 0.0f;

	Matrix4x4 m_world = {};
	Matrix4x4 m_view = {};
	Matrix4x4 m_projection = {};

	Matrix4x4 m_light_rotation = {};
	Vector4 m_light_position = {};

	float m_camera_distance = 0.0f;
	float m_current_camera_distance = 0.0f;
	Vector3 m_camera_rotation = {};
	Vector3 m_current_camera_rotation = {};
	Vector3 m_camera_position = {};

	UINT m_apply_index = 0;
	float m_life_time = 0.0f;
	float m_render_time = 0.0f;
};

