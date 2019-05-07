#pragma once
#include <cstdint>

enum NutexImageFormat : uint8_t
{
	R8G8B8A8_UNORM = 0x00,
	R8G8B8A8_SRGB = 0x05,
	R32G32B32A32_FLOAT = 0x34,
	B8G8R8A8_UNORM = 0x50,
	B8G8R8A8_SRGB = 0x55,
	BC1_UNORM = 0x80,
	BC1_SRGB = 0x85,
	BC2_UNORM = 0x90,
	BC2_SRGB = 0x95,
	BC3_UNORM = 0xa0,
	BC3_SRGB = 0xa5,
	BC4_UNORM = 0xb0,
	BC4_SNORM = 0xb5,
	BC5_UNORM = 0xc0,
	BC5_SNORM = 0xc5,
	BC6_UFLOAT = 0xd7,
	BC7_UNORM = 0xe0,
	BC7_SRGB = 0xe5,
};