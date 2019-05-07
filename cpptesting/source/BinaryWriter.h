#pragma once
#include <string>
#include <fstream>
#include <cstdint>

class BinaryWriter
{

public:

	BinaryWriter(const std::string& filename);
	~BinaryWriter();

	void writeUint8(uint8_t data);
	void writeUint16(uint16_t data);
	void writeUint32(uint32_t data);
	void writeUint64(uint64_t data);

	void writeInt8(int8_t data);
	void writeInt16(int16_t data);
	void writeInt32(int32_t data);
	void writeInt64(int64_t data);

	void writeSingle(float data);
	void writeDouble(double data);

	void writeUint8s(uint8_t* data, const int count);
	void writeUint16s(uint16_t* data, const int count);
	void writeUint32s(uint32_t* data, const int count);
	void writeUint64s(uint64_t* data, const int count);

	void writeInt8s(int8_t* data, const int count);
	void writeInt16s(int16_t* data, const int count);
	void writeInt32s(int32_t* data, const int count);
	void writeInt64s(int64_t* data, const int count);

	void writeSingles(float* data, const int count);
	void writeDoubles(double* data, const int count);

private:

	std::ofstream m_file;

	std::string m_filename;
};