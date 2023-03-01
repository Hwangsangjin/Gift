#pragma once

#include "Point.h"

class Window;

class Input
{
public:
	static Input* GetInstance();

	void Update();
	void AddListener(Window* listener);
	void RemoveListener(Window* listener);

	void SetCursorPosition(const Point& point);
	void ShowCursor(bool show);

private:
	std::unordered_set<Window*> m_set_listeners;
	UCHAR m_key_states[256] = {};
	UCHAR m_old_key_states[256] = {};
	Point m_old_mouse_position = {};
	bool m_first_time = true;
};

