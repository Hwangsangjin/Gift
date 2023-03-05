#pragma once

#include "Rect.h"

class Vector2;

class Input
{
public:
	Input();
	~Input();

	void Update();

	void LockCursor(bool lock);
	void SetLockArea(const Rect& area);

	bool IsKeyUp(const Key& key);
	bool IsKeyDown(const Key& key);

	Vector2 GetDeltaMousePosition();

private:
	short GetKeyCode(const Key& key);

	short m_key_states[256] = {};
	short m_old_key_states[256] = {};
	short m_final_key_states[256] = {};

	bool m_cursor_locked = false;

	Rect m_lock_area = {};
	Vector2 m_lock_area_center = {};

	Vector2 m_old_mouse_position = {};
	Vector2 m_delta_mouse_position = {};
};

