#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>

struct ByamlHeader
{
	char magic[2];
	char version[2];
	uint32_t nodeNameTableNode;
	uint32_t stringValueTableNode;
	uint32_t pathValueTableNode;
	uint32_t rootNode;

	// returns true if this is a valid ByamlHeader
	bool isValidHeader()
	{
		return (magic[0] == 'B' && magic[1] == 'Y') || (magic[0] == 'Y' && magic[1] == 'B');
	}
};

struct ByamlStringTableNode
{
	unsigned char magic; // 0xc2
	uint32_t stringCount; // 3 bytes
	uint32_t* stringOffsets;

	bool isValidStringTableNode()
	{
		return magic == 0xc2;
	}
};

struct ByamlPathTableNode
{
	unsigned char magic; // 0xc3
	uint32_t pathCount; // 3 bytes
	uint32_t* pathOffsets;

	bool isValidPathTableNode()
	{
		return magic == 0xc3;
	}
};

struct ByamlPath
{
	float positionX;
	float positionY;
	float positionZ;
	float normalX;
	float normalY;
	float normalZ;
	uint32_t unknown;

	friend std::ostream& operator << (std::ostream& o, const ByamlPath& path)
	{
		o << "Position: (" << path.positionX << ", " << path.positionY << ", " << path.positionZ << ")" << std::endl;
		o << "Normal: (" << path.normalX << ", " << path.normalY << ", " << path.normalZ << ")" << std::endl;

		return o;
	}
};

class Byaml
{
public:
	Byaml(const char* path);
	~Byaml();

private:

	std::ifstream m_file;

	ByamlHeader m_header;
	ByamlStringTableNode m_nodeNameTableNode;
	ByamlStringTableNode m_stringValueTableNode;
	ByamlPathTableNode m_pathTableNode;
};