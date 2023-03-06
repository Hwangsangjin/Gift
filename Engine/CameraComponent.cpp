#include "pch.h"
#include "CameraComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "World.h"
#include "App.h"
#include "Graphics.h"

CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
	m_entity->GetWorld()->GetApp()->GetGraphics()->RemoveComponent(this);
}

float CameraComponent::GetFieldOfView()
{
	return m_field_of_view;
}

float CameraComponent::GetNearPlane()
{
	return m_near_plane;
}

float CameraComponent::GetFarPlane()
{
	return m_far_plane;
}

void CameraComponent::SetFieldOfView(float field_of_view)
{
	m_field_of_view = field_of_view;
	ComputeProjectionMatrix();
}

void CameraComponent::SetNearPlane(float near_plane)
{
	m_near_plane = near_plane;
	ComputeProjectionMatrix();
}

void CameraComponent::SetFarPlane(float far_plane)
{
	m_far_plane = far_plane;
	ComputeProjectionMatrix();
}

void CameraComponent::SetCameraType(const CameraType& camera_type)
{
	m_camera_type = camera_type;
	ComputeProjectionMatrix();
}

void CameraComponent::SetScreenArea(const Rect& screen_area)
{
	m_screen_area = screen_area;
	ComputeProjectionMatrix();
}

CameraType CameraComponent::GetCameraType()
{
	return m_camera_type;
}

Rect CameraComponent::GetScreenArea()
{
	return m_screen_area;
}

void CameraComponent::GetViewMatrix(Matrix4x4& view)
{
	m_entity->GetTransform()->GetWorldMatrix(view);
	view.Inverse();
}

void CameraComponent::GetProjectionMatrix(Matrix4x4& projection)
{
	projection = m_projection;
}

void CameraComponent::OnCreate()
{
	m_entity->GetWorld()->GetApp()->GetGraphics()->AddComponent(this);
}

void CameraComponent::ComputeProjectionMatrix()
{
	if (m_camera_type == CameraType::Perspective)
		m_projection.SetPerspective(m_field_of_view, static_cast<float>(m_screen_area.m_width) / static_cast<float>(m_screen_area.m_height), m_near_plane, m_far_plane);
	else if (m_camera_type == CameraType::Orthorthographic)
		m_projection.SetOrthographic(static_cast<float>(m_screen_area.m_width), static_cast<float>(m_screen_area.m_height), m_near_plane, m_far_plane);
}
