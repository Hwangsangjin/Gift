#include "pch.h"
#include "Point.h"

Point::Point(int x, int y)
	: m_x(x)
	, m_y(y)
{
}

Point::Point(const Point& point)
	: m_x(point.m_x)
	, m_y(point.m_y)
{
}

int Point::GetX() const
{
	return m_x;
}

int Point::GetY() const
{
	return m_y;
}
