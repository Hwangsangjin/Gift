#include "pch.h"
#include "Object.h"
#include "World.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "InputSystem.h"
#include "Timer.h"

Object::Object()
{
}

Object::~Object()
{
}

void Object::OnCreate()
{
	Entity::OnCreate();

	auto camera = CreateComponent<CameraComponent>();
	camera->SetFarPlane(2000.0f);

	GetTransform()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
}

void Object::OnUpdate(float delta_time)
{
	Entity::OnUpdate(delta_time);

	auto transform = GetTransform();

	m_upward = 0.0f;
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (GetInputSystem()->IsKeyDown(Key::Q))
		m_upward = -1.0f;
	if (GetInputSystem()->IsKeyDown(Key::E))
		m_upward = 1.0f;
	if (GetInputSystem()->IsKeyDown(Key::W))
		m_forward = 1.0f;
	if (GetInputSystem()->IsKeyDown(Key::S))
		m_forward = -1.0f;
	if (GetInputSystem()->IsKeyDown(Key::A))
		m_rightward = -1.0f;
	if (GetInputSystem()->IsKeyDown(Key::D))
		m_rightward = 1.0f;

	auto delta_mouse_position = GetInputSystem()->GetDeltaMousePosition();
	auto rotation = transform -> GetRotation();
	rotation = rotation + Vector3(delta_mouse_position.m_y * 0.001f, delta_mouse_position.m_x * 0.001f, 0.0f);

	Matrix4x4 world;
	transform->GetWorldMatrix(world);

	auto position = transform->GetPosition();
	position = position + world.GetYDirection() * m_upward * 8.0f * delta_time;
	position = position + world.GetZDirection() * m_forward * 8.0f * delta_time;
	position = position + world.GetXDirection() * m_rightward * 8.0f * delta_time;

	transform->SetPosition(position);
	transform->SetRotation(rotation);
}