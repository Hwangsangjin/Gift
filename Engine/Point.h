#pragma once

class Point
{
public:
	Point() = default;
	Point(int x, int y);
	Point(const Point& point);

	int m_x = 0;
	int m_y = 0;
};

