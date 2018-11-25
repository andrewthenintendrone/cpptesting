#pragma once
#include "Array2D.h"

struct Point
{
	Point()
	{
		dx = 9999;
		dy = 9999;
	};
	Point(int x, int y) : dx(x), dy(y) {}

	int dx;
	int dy;

	int DistSq() const { return dx * dx + dy * dy; }
};

struct Grid
{
	Grid(unsigned int width, unsigned int height)
	{
		grid = Array2D<Point>(width, height);
	}

	Point Get(int x, int y)
	{
		return grid(x, y);
	}

	void Set(int x, int y, Point p)
	{
		grid(x, y) = p;
	}

	void Compare(Point& p, int x, int y, int offsetx, int offsety)
	{
		Point other = Get(x + offsetx, y + offsety);
		other.dx += offsetx;
		other.dy += offsety;

		if (other.DistSq() < p.DistSq())
		{
			p = other;
		}
	}

	void GenerateSDF()
	{
		// Pass 0
		for (unsigned int y = 0; y < grid.getHeight(); y++)
		{
			for (unsigned int x = 0; x < grid.getWidth(); x++)
			{
				Point p = Get(x, y);
				Compare(p, x, y, -1, 0);
				Compare(p, x, y, 0, -1);
				Compare(p, x, y, -1, -1);
				Compare(p, x, y, 1, -1);
				Set(x, y, p);
			}

			for (int x = grid.getWidth() - 1; x >= 0; x--)
			{
				Point p = Get(x, y);
				Compare(p, x, y, 1, 0);
				Set(x, y, p);
			}
		}

		// Pass 1
		for (int y = grid.getHeight() - 1; y >= 0; y--)
		{
			for (int x = grid.getWidth() - 1; x >= 0; x--)
			{
				Point p = Get(x, y);
				Compare(p, x, y, 1, 0);
				Compare(p, x, y, 0, 1);
				Compare(p, x, y, -1, 1);
				Compare(p, x, y, 1, 1);
				Set(x, y, p);
			}

			for (unsigned int x = 0; x < grid.getWidth(); x++)
			{
				Point p = Get(x, y);
				Compare(p, x, y, -1, 0);
				Set(x, y, p);
			}
		}
	}

	Array2D<Point> grid;
};