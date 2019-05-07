#pragma once
#include <string>
#include <fstream>
#include <cstdint>

class BinaryReader
{

public:

	BinaryReader(const std::string& filename);
	~BinaryReader();

	uint8_t readUint8();
	uint16_t readUint16();
	uint32_t readUint32();
	uint64_t readUint64();

	int8_t readInt8();
	int16_t readInt16();
	int32_t readInt32();
	int64_t readInt64();

	float readSingle();
	double readDouble();

	uint8_t* readUint8s(const int count);
	uint16_t* readUint16s(const int count);
	uint32_t* readUint32s(const int count);
	uint64_t* readUint64s(const int count);

	int8_t* readInt8s(const int count);
	int16_t* readInt16s(const int count);
	int32_t* readInt32s(const int count);
	int64_t* readInt64s(const int count);

	float* readSingles(const int count);
	double* readDoubles(const int count);

	std::streampos getFileSize() const { return m_filesize; }
	std::streampos getFileOffset() { return m_file.tellg(); }

private:

	std::ifstream m_file;

	std::streampos m_filesize;

	std::string m_filename;
};