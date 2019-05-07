#include "BinaryReader.h"
#include <iostream>

BinaryReader::BinaryReader(const std::string& filename)
{
	m_filename = filename;

	m_file.open(m_filename, std::ios::_Nocreate | std::ios::ate | std::ios::binary);

	if (!m_file.is_open())
	{
		std::cout << "Failed to open " << m_filename << std::endl;
		return;
	}

	m_filesize = m_file.tellg();

	// jump back to beginning
	m_file.seekg(0, std::ios::beg);
}

BinaryReader::~BinaryReader()
{
	// close file if it's open
	if (m_file.is_open())
	{
		m_file.close();
	}
}

uint8_t BinaryReader::readUint8()
{
	uint8_t data;

	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

	return data;
}

uint16_t BinaryReader::readUint16()
{
	uint16_t data;

	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

	return data;
}

uint32_t BinaryReader::readUint32()
{
	uint32_t data;

	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

	return data;
}

uint64_t BinaryReader::readUint64()
{
	uint64_t data;

	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

	return data;
}

int8_t BinaryReader::readInt8()
{
	int8_t data;

	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

	return data;
}

int16_t BinaryReader::readInt16()
{
	int16_t data;

	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

	return data;
}

int32_t BinaryReader::readInt32()
{
	int32_t data;

	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

	return data;
}

int64_t BinaryReader::readInt64()
{
	int64_t data;

	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

	return data;
}

float BinaryReader::readSingle()
{
	float data;

	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

	return data;
}

double BinaryReader::readDouble()
{
	double data;

	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

	return data;
}

uint8_t* BinaryReader::readUint8s(const int count)
{
	uint8_t* data = new uint8_t[count];

	m_file.read(reinterpret_cast<char*>(data), sizeof(uint8_t) * count);

	return data;
}

uint16_t* BinaryReader::readUint16s(const int count)
{
	uint16_t* data = new uint16_t[count];

	m_file.read(reinterpret_cast<char*>(data), sizeof(uint16_t) * count);

	return data;
}

uint32_t* BinaryReader::readUint32s(const int count)
{
	uint32_t* data = new uint32_t[count];

	m_file.read(reinterpret_cast<char*>(data), sizeof(uint32_t) * count);

	return data;
}

uint64_t* BinaryReader::readUint64s(const int count)
{
	uint64_t* data = new uint64_t[count];

	m_file.read(reinterpret_cast<char*>(data), sizeof(uint64_t) * count);

	return data;
}

int8_t* BinaryReader::readInt8s(const int count)
{
	int8_t* data = new int8_t[count];

	m_file.read(reinterpret_cast<char*>(data), sizeof(int8_t) * count);

	return data;
}

int16_t* BinaryReader::readInt16s(const int count)
{
	int16_t* data = new int16_t[count];

	m_file.read(reinterpret_cast<char*>(data), sizeof(int16_t) * count);

	return data;
}

int32_t* BinaryReader::readInt32s(const int count)
{
	int32_t* data = new int32_t[count];

	m_file.read(reinterpret_cast<char*>(data), sizeof(int32_t) * count);

	return data;
}

int64_t* BinaryReader::readInt64s(const int count)
{
	int64_t* data = new int64_t[count];

	m_file.read(reinterpret_cast<char*>(data), sizeof(int64_t) * count);

	return data;
}

float* BinaryReader::readSingles(const int count)
{
	float* data = new float[count];

	m_file.read(reinterpret_cast<char*>(data), sizeof(float) * count);

	return data;
}

double* BinaryReader::readDoubles(const int count)
{
	double* data = new double[count];

	m_file.read(reinterpret_cast<char*>(data), sizeof(double) * count);

	return data;
}