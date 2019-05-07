#pragma once
#include "Color.h"

enum WRAPTYPE
{
	CLAMP,
	WRAP
};

class Image
{
public:

	Image() {}
	Image(int width, int height, int channels);
	Image(const char* filename);
	Image(unsigned char* buffer, int width, int height, int channels);
	~Image();

	void load(const char* filename);
	void save(const char* filename);

	void saveSignedDistanceField(const char* filename);

	Color getPixel(int x, int y, WRAPTYPE wrapType) const;
	void setPixel(int x, int y, Color color, WRAPTYPE wrapType);

	void sort();

	Image sobelEdgeDetection();
	Image gaussianBlur();

	// overloads
	void operator = (const Image& other);
	Image operator * (const Image& other) const;
	void operator *= (const Image& other);
	Image operator + (const Image& other) const;
	void operator += (const Image& other);
	Image operator - (const Image& other) const;

	const int getWidth() { return m_width; }
	const int getHeight() { return m_height; }
	const int getChannelCount() { return m_channels; }

protected:

	char* m_filename = nullptr;
	unsigned char* m_pixelbuffer = nullptr;
	int m_width = 0;
	int m_height = 0;
	int m_channels = 0;

	unsigned int getPixelOffset(int x, int y, WRAPTYPE wrapType) const;
};