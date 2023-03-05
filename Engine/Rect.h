#pragma once

class Rect
{
public:
	Rect() = default;
	Rect(UINT width, UINT height);
	Rect(UINT width, UINT height, UINT left, UINT top);
	Rect(const Rect& rect);

	UINT m_left = 0;
	UINT m_top = 0;
	UINT m_width = 0;
	UINT m_height = 0;
};

