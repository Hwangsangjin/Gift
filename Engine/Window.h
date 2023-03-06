#pragma once

#include "Rect.h"

class Point;

class Window
{
public:
	Window();
	virtual ~Window();

	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnSize(const Rect& size);
	virtual void OnFocus();
	virtual void OnKillFocus();
	virtual void OnDestroy();

	Rect GetClientSize() const;
	Rect GetScreenSize() const;

protected:
	void* m_hwnd = nullptr;
	Rect m_size = { 0, 0, 1024, 768 };
};

