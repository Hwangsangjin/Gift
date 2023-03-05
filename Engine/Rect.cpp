#include "pch.h"
#include "Rect.h"

Rect::Rect(UINT width, UINT height)
	: m_left(0)
	, m_top(0)
	, m_width(width)
	, m_height(height)
{
}

Rect::Rect(UINT left, UINT top, UINT width, UINT height)
	: m_left(left)
	, m_top(top)
	, m_width(width)
	, m_height(height)
{
}

Rect::Rect(const Rect& rect)
	: m_left(rect.m_left)
	, m_top(rect.m_top)
	, m_width(rect.m_width)
	, m_height(rect.m_height)
{
}
