#pragma once

#include "Component.h"
#include "Rect.h"
#include "Matrix4x4.h"

class CameraComponent : public Component
{
public:
	CameraComponent();
	virtual ~CameraComponent();

	float GetFieldOfView();
	float GetNearPlane();
	float GetFarPlane();

	void SetFieldOfView(float field_of_view);
	void SetNearPlane(float near_plane);
	void SetFarPlane(float far_plane);

	void SetCameraType(const CameraType& camera_type);
	void SetScreenArea(const Rect& screen_area);

	CameraType GetCameraType();
	Rect GetScreenArea();

	void GetViewMatrix(Matrix4x4& view);
	void GetProjectionMatrix(Matrix4x4& projection);

protected:
	virtual void OnCreate() override;

private:
	void ComputeProjectionMatrix();

	float m_field_of_view = 1.3f;
	float m_near_plane = 0.01f;
	float m_far_plane = 100.0f;

	CameraType m_camera_type = CameraType::Perspective;
	Rect m_screen_area;

	Matrix4x4 m_projection = {};
};

