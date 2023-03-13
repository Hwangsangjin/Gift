#pragma once

#include "Component.h"
#include "Vector3.h"
#include "Matrix4x4.h"

class TransformComponent : public Component
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	Vector3 GetPosition();
	Vector3 GetRotation();
	Vector3 GetScale();

	void SetPosition(const Vector3& position);
	void SetRotation(const Vector3& rotation);
	void SetScale(const Vector3& scale);

	void GetWorldMatrix(Matrix4x4& matrix) const;
	void SetBillboardMatrix(const Matrix4x4& matrix);

protected:
	void UpdateWorldMatrix();

	Vector3 m_position = {};
	Vector3 m_rotation = {};
	Vector3 m_scale = Vector3(1.0f, 1.0f, 1.0f);

	Matrix4x4 m_world_matrix = {};
};

