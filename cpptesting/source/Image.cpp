#include "Image.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb\stb_image_write.h>

#include "SignedDistanceField.h"

#include<vector>

// constructor from file
Image::Image(const char* filename)
{
	load(filename);
}

Image::Image(unsigned char* buffer, int width, int height, int channels)
{
	m_pixelbuffer = buffer;
	m_width = width;
	m_height = height;
	m_channels = channels;
}

// constructor for blank image
Image::Image(int width, int height, int channels)
{
	m_width = width;
	m_height = height;
	m_channels = channels;

	m_pixelbuffer = new unsigned char[m_width * m_height * m_channels];
}

// destructor
Image::~Image()
{
	if (m_pixelbuffer)
	{
		stbi_image_free(m_pixelbuffer);
		m_pixelbuffer = nullptr;
	}
}

// load image
void Image::load(const char* filename)
{
	// attempt to load image
	m_pixelbuffer = stbi_load(filename, &m_width, &m_height, &m_channels, 0);

	// check that image was loaded
	if (m_pixelbuffer)
	{
		m_filename = (char*)filename;
	}
	else
	{
		std::cout << "Failed to load " << filename << std::endl;
		stbi_image_free(m_pixelbuffer);
		m_pixelbuffer = nullptr;
	}
}

// save image
void Image::save(const char* filename)
{
	stbi_write_jpg(filename, m_width, m_height, m_channels, m_pixelbuffer, m_width * m_channels);
}

// save image as a signed distance field
void Image::saveSignedDistanceField(const char* filename)
{
	// convert m_pixelbuffer to signed distance field
	Grid grid1(m_width, m_height);
	Grid grid2(m_width, m_height);

	Point inside{ 0, 0 };
	Point empty{ 9999, 9999 };

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int g = m_pixelbuffer[m_channels * (y * m_width + x) + 3];

			// Points inside get marked with a dx/dy of zero.
			// Points outside get marked with an infinitely large distance.
			if (g < 128)
			{
				grid1.Set(x, y, inside);
				grid2.Set(x, y, empty);
			}
			else
			{
				grid2.Set(x, y, inside);
				grid1.Set(x, y, empty);
			}
		}
	}

	grid1.GenerateSDF();
	grid2.GenerateSDF();

	// combine pass
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int dist1 = (int)(sqrt((double)grid1.Get(x, y).DistSq()));
			int dist2 = (int)(sqrt((double)grid2.Get(x, y).DistSq()));
			int dist = dist1 - dist2;

			// Clamp and scale it, just for display purposes.
			int c = dist * 3 + 128;
			c = std::min(std::max(c, 0), 255);

			m_pixelbuffer[m_channels * (y * m_width + x)] = c;
			m_pixelbuffer[m_channels * (y * m_width + x) + 1] = c;
			m_pixelbuffer[m_channels * (y * m_width + x) + 2] = c;
			m_pixelbuffer[m_channels * (y * m_width + x) + 3] = 255;
		}
	}

	save(filename);
}

Color Image::getPixel(int x, int y, WRAPTYPE wrapType) const
{
	unsigned int pixelOffset = getPixelOffset(x, y, wrapType);

	Color pixel;

	pixel.r = m_pixelbuffer[pixelOffset];
	pixel.g = m_pixelbuffer[pixelOffset + 1];
	pixel.b = m_pixelbuffer[pixelOffset + 2];

	// alpha
	if (m_channels == 4)
	{
		pixel.a = m_pixelbuffer[pixelOffset + 3];
	}

	return pixel;
}

void Image::setPixel(int x, int y, Color color, WRAPTYPE wrapType)
{
	unsigned int pixelOffset = getPixelOffset(x, y, wrapType);

	m_pixelbuffer[pixelOffset] = color.r;
	m_pixelbuffer[pixelOffset + 1] = color.g;
	m_pixelbuffer[pixelOffset + 2] = color.b;

	// alpha
	if (m_channels == 4)
	{
		m_pixelbuffer[pixelOffset + 3] = color.a;
	}
}

// sorts colors based on total rgb
bool colorSort(Color& a, Color& b)
{
	if (a.a < b.a)
	{
		return true;
	}

	return a.getHue() < b.getHue();
}

void Image::sort()
{
	std::vector<Color> sortedPixels;

	// extract colors from pixel buffer
	for (size_t y = 0; y < m_height; y++)
	{
		for (size_t x = 0; x < m_width; x++)
		{
			Color currentColor;
			int pixelOffset = getPixelOffset(x, y, WRAPTYPE::CLAMP);

			currentColor.r = m_pixelbuffer[pixelOffset];
			currentColor.g = m_pixelbuffer[pixelOffset + 1];
			currentColor.b = m_pixelbuffer[pixelOffset + 2];

			// alpha
			if (m_channels == 4)
			{
				currentColor.a = m_pixelbuffer[pixelOffset + 3];
			}

			sortedPixels.push_back(currentColor);
		}
	}

	// sort color array
	std::sort(sortedPixels.begin(), sortedPixels.end(), colorSort);

	// put colors back into pixel buffer
	for (int y = 0, i = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++, i++)
		{
			int pixelOffset = getPixelOffset(x, y, WRAPTYPE::CLAMP);

			m_pixelbuffer[pixelOffset] = sortedPixels[i].r;
			m_pixelbuffer[pixelOffset + 1] = sortedPixels[i].g;
			m_pixelbuffer[pixelOffset + 2] = sortedPixels[i].b;

			if (m_channels == 4)
			{
				m_pixelbuffer[pixelOffset + 3] = sortedPixels[i].a;
			}
		}
	}
}

Image Image::sobelEdgeDetection()
{
	Image* sobelImage = new Image(m_width, m_height, m_channels);

	float kernelX[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	float kernelY[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

	glm::vec3 currentColorX(0, 0, 0);
	glm::vec3 currentColorY(0, 0, 0);

	// loop through pixels
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			for (int dx = -1, i = 0; dx < 2; dx++)
			{
				for (int dy = -1; dy < 2; dy++, i++)
				{
					glm::vec3 currentPixel = getPixel(x + dx, y + dy, WRAPTYPE::CLAMP).asVec3();

					currentColorX += currentPixel * kernelX[i];
					currentColorY += currentPixel * kernelY[i];
				}
			}

			// average and apply pixel

			glm::vec3 finalColor = currentColorX * currentColorX + currentColorY * currentColorY;

			finalColor.r = std::fmaxf(std::fminf(std::powf(finalColor.r, 0.5f), 1.0f), 0.0f);
			finalColor.g = std::fmaxf(std::fminf(std::powf(finalColor.g, 0.5f), 1.0f), 0.0f);
			finalColor.b = std::fmaxf(std::fminf(std::powf(finalColor.b, 0.5f), 1.0f), 0.0f);

			uint8_t v = 255 - (uint8_t)(std::fminf(std::fmaxf(((finalColor.r + finalColor.g + finalColor.b) / 3.0f), 0.0f), 1.0f) * 255.0f);

			sobelImage->setPixel(x, y, Color(v, v, v, 255), WRAPTYPE::CLAMP);

			currentColorX = glm::vec3(0, 0, 0);
			currentColorY = glm::vec3(0, 0, 0);
		}
	}

	return *sobelImage;
}

Image Image::gaussianBlur()
{
	Image* blurredImage = new Image(m_width, m_height, m_channels);

	float kernel[7] = { 1.0f, 6.0f, 15.0f, 20.0f, 15.0f, 6.0f, 1.0f };

	if (m_channels == 3)
	{
		glm::vec3 currentColor(0, 0, 0);

		// loop through pixels
		for (int x = 0; x < m_width; x++)
		{
			for (int y = 0; y < m_height; y++)
			{
				for (int dx = -3, i = 0; dx < 4; dx++)
				{
					for (int dy = -3; dy < 4; dy++, i++)
					{
						glm::vec3 currentPixel = getPixel(x + dx, y + dy, WRAPTYPE::CLAMP).asVec3();

						currentColor += currentPixel * ((kernel[dx + 3] * kernel[dy + 3]) / 4096.0f);
					}
				}

				// average and apply pixel

				currentColor.r = std::fmaxf(std::fminf(currentColor.r, 1.0f), 0.0f);
				currentColor.g = std::fmaxf(std::fminf(currentColor.g, 1.0f), 0.0f);
				currentColor.b = std::fmaxf(std::fminf(currentColor.b, 1.0f), 0.0f);

				blurredImage->setPixel(x, y, Color(currentColor), WRAPTYPE::CLAMP);

				currentColor = glm::vec3(0, 0, 0);
			}
		}
	}
	else if (m_channels == 4)
	{
		glm::vec4 currentColor(0, 0, 0, 0);

		// loop through pixels
		for (int x = 0; x < m_width; x++)
		{
			for (int y = 0; y < m_height; y++)
			{
				for (int dx = -3, i = 0; dx < 4; dx++)
				{
					for (int dy = -3; dy < 4; dy++, i++)
					{
						glm::vec4 currentPixel = getPixel(x + dx, y + dy, WRAPTYPE::CLAMP).asVec4();

						currentColor += currentPixel * ((kernel[dx + 3] * kernel[dy + 3]) / 4096.0f);
					}
				}

				// average and apply pixel

				currentColor.r = std::fmaxf(std::fminf(currentColor.r, 1.0f), 0.0f);
				currentColor.g = std::fmaxf(std::fminf(currentColor.g, 1.0f), 0.0f);
				currentColor.b = std::fmaxf(std::fminf(currentColor.b, 1.0f), 0.0f);
				currentColor.a = std::fmaxf(std::fminf(currentColor.a, 1.0f), 0.0f);

				blurredImage->setPixel(x, y, Color(currentColor), WRAPTYPE::CLAMP);

				currentColor = glm::vec4(0, 0, 0, 0);
			}
		}
	}

	return *blurredImage;
}

void Image::operator = (const Image& other)
{
	// clean up if necessary
	if (m_pixelbuffer)
	{
		stbi_image_free(m_pixelbuffer);
		m_pixelbuffer = nullptr;
	}

	m_width = other.m_width;
	m_height = other.m_height;
	m_channels = other.m_channels;
	m_filename = other.m_filename;
	
	for (size_t i = 0; i < (m_width * m_height); i++)
	{
		m_pixelbuffer[i] = other.m_pixelbuffer[i];
	}
}

Image Image::operator * (const Image& other) const 
{
	Image i = *this;

	// ensure same size
	if (m_width == other.m_width && m_height == other.m_height && m_channels == other.m_channels)
	{
		std::cout << "ok" << std::endl;

		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				i.setPixel(x, y, getPixel(x, y, WRAPTYPE::CLAMP) * other.getPixel(x, y, WRAPTYPE::CLAMP), WRAPTYPE::CLAMP);
			}
		}
	}

	return i;
}

void Image::operator *= (const Image& other)
{
	// ensure same size
	if (m_width == other.m_width && m_height == other.m_height && m_channels == other.m_channels)
	{
		std::cout << "ok" << std::endl;

		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				setPixel(x, y, getPixel(x, y, WRAPTYPE::CLAMP) * other.getPixel(x, y, WRAPTYPE::CLAMP), WRAPTYPE::CLAMP);
			}
		}
	}
}

Image Image::operator + (const Image& other) const
{
	Image i = *this;

	// ensure same size
	if (m_width == other.m_width && m_height == other.m_height && m_channels == other.m_channels)
	{
		std::cout << "ok" << std::endl;

		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				i.setPixel(x, y, getPixel(x, y, WRAPTYPE::CLAMP) + other.getPixel(x, y, WRAPTYPE::CLAMP), WRAPTYPE::CLAMP);
			}
		}
	}

	return i;
}

void Image::operator += (const Image& other)
{
	// ensure same size
	if (m_width == other.m_width && m_height == other.m_height && m_channels == other.m_channels)
	{
		std::cout << "ok" << std::endl;

		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				setPixel(x, y, getPixel(x, y, WRAPTYPE::CLAMP) + other.getPixel(x, y, WRAPTYPE::CLAMP), WRAPTYPE::CLAMP);
			}
		}
	}
}

Image Image::operator - (const Image& other) const
{
	Image i = *this;

	// ensure same size
	if (m_width == other.m_width && m_height == other.m_height && m_channels == other.m_channels)
	{
		std::cout << "ok" << std::endl;

		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				i.setPixel(x, y, getPixel(x, y, WRAPTYPE::CLAMP) - other.getPixel(x, y, WRAPTYPE::CLAMP), WRAPTYPE::CLAMP);
			}
		}
	}

	return i;
}

unsigned int Image::getPixelOffset(int x, int y, WRAPTYPE wrapType) const
{
	if (wrapType == WRAPTYPE::CLAMP)
	{
		// clamp location
		x = std::max<int>(x, 0);
		x = std::min<int>(x, m_width - 1);

		y = std::max<int>(y, 0);
		y = std::min<int>(y, m_height - 1);
	}
	else if (wrapType = WRAPTYPE::WRAP)
	{
		// wrap location
		x = x % m_width;
		y = y % m_height;
	}

	return (m_channels * (y * m_width + x));
}
