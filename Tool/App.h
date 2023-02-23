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
	void UpdateCamera();
	void UpdateModel(Vector3 position, const MaterialPtr& material);
	void UpdateSkyBox();
	void UpdateLight();
	void UpdateUI(Vector3 position, const SpritePtr& sprite);

	void Render();

	void DrawMesh(const MeshPtr& mesh, const MaterialPtr& material);
	void DrawSprite(const SpritePtr& sprite);

private:
	SwapChainPtr m_swap_chain = nullptr;
	VertexBufferPtr m_vertex_buffer = nullptr;
	VertexShaderPtr m_vertex_shader = nullptr;
	PixelShaderPtr m_pixel_shader = nullptr;
	PixelShaderPtr m_skybox_pixel_shader = nullptr;
	ConstantBufferPtr m_constant_buffer = nullptr;
	ConstantBufferPtr m_skybox_constant_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;

	TexturePtr m_plane_texture = nullptr;
	TexturePtr m_start_texture = nullptr;
	TexturePtr m_map_texture = nullptr;
	TexturePtr m_exit_texture = nullptr;
	TexturePtr m_skybox_texture = nullptr;

	MeshPtr m_plane_mesh = nullptr;
	MeshPtr m_skybox_mesh = nullptr;

	MaterialPtr m_plane_material = nullptr;
	MaterialPtr m_skybox_material = nullptr;

	SpritePtr m_plane_sprite = nullptr;
	SpritePtr m_start_sprite = nullptr;
	SpritePtr m_map_sprite = nullptr;
	SpritePtr m_exit_sprite = nullptr;

	float m_rotation_x = 0.0f;
	float m_rotation_y = 0.0f;
	float m_light_rotation_y = 0.0f;
	float m_light_radius = 4.0f;
	float m_upward = 0.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	Matrix4x4 m_world = {};
	Matrix4x4 m_view = {};
	Matrix4x4 m_projection = {};

	bool m_play_state = false;
	bool m_fullscreen_state = false;
	bool m_projection_state = false;

	Vector4 m_light_position = {};
};

