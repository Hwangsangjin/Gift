#include "pch.h"
#include "Rect.h"

Rect::Rect(UINT width, UINT height)
	: m_width(width)
	, m_height(height)
{
}

Rect::Rect(UINT width, UINT height, UINT left, UINT top)
	: m_width(width)
	, m_height(height)
	, m_left(left)
	, m_top(top)
{
}

Rect::Rect(const Rect& rect)
	: m_width(rect.m_width)
	, m_height(rect.m_height)
	, m_left(rect.m_left)
	, m_top(rect.m_top)
{
}
