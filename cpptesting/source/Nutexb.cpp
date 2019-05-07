#include "Nutexb.h"

Nutexb::Nutexb(const std::string& filename)
{
	open(filename);
}

Nutexb::Nutexb(char* filename)
{
	open(filename);
}

void Nutexb::open(const std::string& filename)
{
	m_file.open(filename, std::ios::_Nocreate | std::ios::binary | std::ios::ate);

	if (!m_file.is_open())
	{
		char buffer[200];

		sprintf(buffer, "Failed to open %s", filename.c_str());

		return;
	}

	m_filesize = m_file.tellg();

	m_file.seekg(0, std::ios::beg);
}

Nutexb::~Nutexb()
{
	if (m_file.is_open())
	{
		m_file.close();
	}
}

void Nutexb::readProperties()
{
	// read magic
	m_file.seekg((int)m_filesize - 7);

	char magic[4] = "   ";

	m_file.read(magic, 3);

	if (strcmp(magic, "XET") == 0)
	{
		printf("Found magic\n");
	}
	else
	{
		printf("Didn't find magic\n");
	}

	// read properties
	m_file.seekg((int)m_filesize - 48);

	m_file.read(reinterpret_cast<char*>(&m_properties), sizeof(m_properties));

	// print properties
	printf("width: %d\n", m_properties.width);
	printf("height: %d\n", m_properties.height);
	printf("depth: %d\n", m_properties.depth);
	printf("mip count: %d\n", m_properties.mipCount);
	printf("alignment: %d\n", m_properties.alignment);
	printf("array count: %d\n", m_properties.arrayCount);
	printf("image size: %d\n", m_properties.imageSize);
}
