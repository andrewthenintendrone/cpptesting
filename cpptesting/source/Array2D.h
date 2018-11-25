#pragma once
#include <vector>

// 2D array
template<class T>
class Array2D
{
public:

	Array2D() {};
	Array2D(unsigned int width, unsigned int height) : m_width(width), m_height(height)
	{
		m_data.resize(m_width * m_height);
	}

	const T& operator () (int x, int y) const
	{
		if (withinBounds(x, y))
		{
			return m_data[y * m_width + x];
		}
		else
		{
			printf("Tried to access elemt (%d, %d) of a (%d, %d) array.\n", x, y, m_width, m_height);
			return m_data.front();
		}
	}

	T& operator () (int x, int y)
	{
		if (withinBounds(x, y))
		{
			return m_data[y * m_width + x];
		}
		else
		{
			printf("Tried to access element (%d, %d) of a (%d, %d) array.\n", x, y, m_width, m_height);
			return m_data.front();
		}
	}

	bool withinBounds(int x, int y) const
	{
		return (x >= 0 && x < m_width && y >= 0 && y < m_height);
	};

	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }

private:

	unsigned int m_width = 0;
	unsigned int m_height = 0;

	std::vector<T> m_data;
};
