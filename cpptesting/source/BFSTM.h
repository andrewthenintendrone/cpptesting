#pragma once
#include <cstdint>

class BFSTM
{
public:

	BFSTM() {};
	BFSTM(const char* filename);

	void readData(const char* filename);

private:

	void printSoundEncoding(uint8_t soundEncoding);
};