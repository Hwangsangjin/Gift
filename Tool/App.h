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
	void UpdateSpaceship();
	void UpdateThirdPersonCamera();
	void UpdateCamera();
	void UpdateLight();
	void UpdateSkyBox();
	void UpdateModel(Vector3 position, Vector3 rotation, Vector3 scale, const std::vector<MaterialPtr>& materials);
	void UpdateUI(Vector3 position, const SpritePtr& sprite);

	void Render();

	void DrawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials);
	void DrawSprite(const SpritePtr& sprite);

private:
	SwapChainPtr m_swap_chain = nullptr;

	MeshPtr m_skybox_mesh = nullptr;
	MeshPtr m_plane_mesh = nullptr;
	MeshPtr m_spaceship_mesh = nullptr;
	MeshPtr m_asteroid_mesh = nullptr;

	TexturePtr m_skybox_texture = nullptr;
	TexturePtr m_background_texture = nullptr;
	TexturePtr m_plane_texture = nullptr;
	TexturePtr m_start_texture = nullptr;
	TexturePtr m_map_texture = nullptr;
	TexturePtr m_exit_texture = nullptr;
	TexturePtr m_spaceship_texture = nullptr;
	TexturePtr m_asteroid_texture = nullptr;

	SpritePtr m_plane_sprite = nullptr;
	SpritePtr m_background_sprite = nullptr;
	SpritePtr m_start_sprite = nullptr;
	SpritePtr m_map_sprite = nullptr;
	SpritePtr m_exit_sprite = nullptr;

	MaterialPtr m_skybox_material = nullptr;
	MaterialPtr m_plane_material = nullptr;
	MaterialPtr m_spaceship_material = nullptr;
	MaterialPtr m_asteroid_material = nullptr;

	std::vector<MaterialPtr> m_materials;

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

	float m_camera_distance = 14.0f;
	float m_current_camera_distance = 0.0f;
	Vector3 m_camera_rotation = {};
	Vector3 m_current_camera_rotation = {};
	Vector3 m_camera_position = {};

	float m_spaceship_speed = 125.0f;
	Vector3 m_spaceship_position = {};
	Vector3 m_current_spaceship_position = {};
	Vector3 m_spaceship_rotation = {};
	Vector3 m_current_spaceship_rotation = {};

	Vector3 m_asteroids_position[200] = {};
	Vector3 m_asteroids_rotation[200] = {};
	Vector3 m_asteroids_scale[200] = {};

	bool m_play_state = false;
	bool m_fullscreen_state = false;
	bool m_projection_state = false;

	bool m_turbo_mode = false;
};

