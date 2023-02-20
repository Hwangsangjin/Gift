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
	void UpdateModel();
	void UpdateSkyBox();
	void UpdateUI();

	void Render();

	void DrawMesh(const MeshPtr& mesh, const VertexShaderPtr& vertex_shader, const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer, const TexturePtr* texture_list, unsigned int texture_count);

private:
	SwapChainPtr m_swap_chain = nullptr;
	VertexBufferPtr m_vertex_buffer = nullptr;
	VertexShaderPtr m_vertex_shader = nullptr;
	PixelShaderPtr m_pixel_shader = nullptr;
	PixelShaderPtr m_skybox_pixel_shader = nullptr;
	ConstantBufferPtr m_constant_buffer = nullptr;
	ConstantBufferPtr m_skybox_constant_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;

	MeshPtr m_wall_mesh = nullptr;
	TexturePtr m_wall_texture = nullptr;

	MeshPtr m_skybox_mesh = nullptr;
	TexturePtr m_skybox_texture = nullptr;

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

	Plane* m_plane = nullptr;
};

