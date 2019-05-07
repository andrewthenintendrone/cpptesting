#include "BinaryWriter.h"
#include <iostream>

BinaryWriter::BinaryWriter(const std::string& filename)
{
	m_filename = filename;

	m_file.open(m_filename, std::ios::trunc | std::ios::binary);

	if (!m_file.is_open())
	{
		std::cout << "Failed to open " << m_filename << std::endl;
		return;
	}
}

BinaryWriter::~BinaryWriter()
{
	if (m_file.is_open())
	{
		m_file.close();
	}
}

void BinaryWriter::writeUint8(uint8_t data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void BinaryWriter::writeUint16(uint16_t data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void BinaryWriter::writeUint32(uint32_t data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void BinaryWriter::writeUint64(uint64_t data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void BinaryWriter::writeInt8(int8_t data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void BinaryWriter::writeInt16(int16_t data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void BinaryWriter::writeInt32(int32_t data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void BinaryWriter::writeInt64(int64_t data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void BinaryWriter::writeSingle(float data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void BinaryWriter::writeDouble(double data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

void BinaryWriter::writeUint8s(uint8_t* data, const int count)
{
	m_file.write(reinterpret_cast<char*>(data), sizeof(uint8_t) * count);
}

void BinaryWriter::writeUint16s(uint16_t* data, const int count)
{
	m_file.write(reinterpret_cast<char*>(data), sizeof(uint16_t) * count);
}

void BinaryWriter::writeUint32s(uint32_t* data, const int count)
{
	m_file.write(reinterpret_cast<char*>(data), sizeof(uint32_t) * count);
}

void BinaryWriter::writeUint64s(uint64_t* data, const int count)
{
	m_file.write(reinterpret_cast<char*>(data), sizeof(uint64_t) * count);
}

void BinaryWriter::writeInt8s(int8_t* data, const int count)
{
	m_file.write(reinterpret_cast<char*>(data), sizeof(int8_t) * count);
}

void BinaryWriter::writeInt16s(int16_t* data, const int count)
{
	m_file.write(reinterpret_cast<char*>(data), sizeof(int16_t) * count);
}

void BinaryWriter::writeInt32s(int32_t* data, const int count)
{
	m_file.write(reinterpret_cast<char*>(data), sizeof(int32_t) * count);
}

void BinaryWriter::writeInt64s(int64_t* data, const int count)
{
	m_file.write(reinterpret_cast<char*>(data), sizeof(int64_t) * count);
}

void BinaryWriter::writeSingles(float* data, const int count)
{
	m_file.write(reinterpret_cast<char*>(data), sizeof(float) * count);
}

void BinaryWriter::writeDoubles(double* data, const int count)
{
	m_file.write(reinterpret_cast<char*>(data), sizeof(double) * count);
}