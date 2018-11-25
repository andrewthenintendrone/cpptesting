#include "Byaml.h"
#include <string>

Byaml::Byaml(const char* path)
{
	// attempt to open the file
	m_file.open(path, std::ios::_Nocreate | std::ios::binary);

	if (!m_file.is_open())
	{
		std::cout << "Failed to open " << path << std::endl;
		return;
	}

	// read the header
	m_file.read(reinterpret_cast<char*>(&m_header), sizeof(ByamlHeader));

	if (!m_header.isValidHeader())
	{
		std::cout << "File " << path << " does not have a valid Byaml Header" << std::endl;
	}

	// jump to node name table node
	m_file.seekg(m_header.nodeNameTableNode, std::ios::beg);

	m_file.read(reinterpret_cast<char*>(&m_nodeNameTableNode.magic), 1);

	if (!m_nodeNameTableNode.isValidStringTableNode())
	{
		std::cout << "node name table node did not have a valid header" << std::endl;
	}

	m_file.read(reinterpret_cast<char*>(&m_nodeNameTableNode.stringCount), 3);

	// allocate memory for string offsets
	m_nodeNameTableNode.stringOffsets = new uint32_t[m_nodeNameTableNode.stringCount];

	m_file.read(reinterpret_cast<char*>(&m_nodeNameTableNode.stringOffsets[0]), sizeof(uint32_t) * m_nodeNameTableNode.stringCount);

	// print each node name
	for (size_t i = 0; i < m_nodeNameTableNode.stringCount; i++)
	{
		m_file.seekg(m_header.nodeNameTableNode + m_nodeNameTableNode.stringOffsets[i], std::ios::beg);

		char c;
		std::string currentNodeName;

		while ((c = m_file.get()) != '\0')
		{
			currentNodeName += c;
		}

		std::cout << currentNodeName << std::endl;
	}

	// deallocate memory for string offsets
	delete[] m_nodeNameTableNode.stringOffsets;

	// read stringValueTableNode (if applicable)
	if (m_header.stringValueTableNode != 0)
	{
		// jump to the string table node
		m_file.seekg(m_header.stringValueTableNode, std::ios::beg);

		m_file.read(reinterpret_cast<char*>(&m_stringValueTableNode.magic), 1);

		if (!m_stringValueTableNode.isValidStringTableNode())
		{
			std::cout << "string value table node did not have a valid header" << std::endl;
		}

		m_file.read(reinterpret_cast<char*>(&m_stringValueTableNode.stringCount), 3);

		// allocate memory for string offsets
		m_stringValueTableNode.stringOffsets = new uint32_t[m_stringValueTableNode.stringCount];

		m_file.read(reinterpret_cast<char*>(&m_stringValueTableNode.stringOffsets[0]), sizeof(uint32_t) * m_stringValueTableNode.stringCount);

		// print each string
		for (size_t i = 0; i < m_stringValueTableNode.stringCount; i++)
		{
			m_file.seekg(m_header.stringValueTableNode + m_stringValueTableNode.stringOffsets[i], std::ios::beg);

			char c;
			std::string currentNodeName;

			while ((c = m_file.get()) != '\0')
			{
				currentNodeName += c;
			}

			std::cout << currentNodeName << std::endl;
		}

		// deallocate memory for string offsets
		delete[] m_stringValueTableNode.stringOffsets;
	}

	// read pathTableNode (if applicable)
	if (m_header.pathValueTableNode != 0)
	{
		// jump to the path table node
		m_file.seekg(m_header.pathValueTableNode, std::ios::beg);

		m_file.read(reinterpret_cast<char*>(&m_pathTableNode.magic), 1);

		if (!m_pathTableNode.isValidPathTableNode())
		{
			std::cout << "path value table node did not have a valid header" << std::endl;
		}

		m_file.read(reinterpret_cast<char*>(&m_pathTableNode.pathCount), 3);

		// allocate memory for path offsets
		m_pathTableNode.pathOffsets = new uint32_t[m_pathTableNode.pathCount];

		m_file.read(reinterpret_cast<char*>(&m_pathTableNode.pathOffsets[0]), sizeof(uint32_t) * m_pathTableNode.pathCount);

		// print each path node
		for (size_t i = 0; i < m_pathTableNode.pathCount; i++)
		{
			m_file.seekg(m_header.pathValueTableNode + m_pathTableNode.pathOffsets[i], std::ios::beg);

			ByamlPath currentPath;

			m_file.read(reinterpret_cast<char*>(&currentPath), sizeof(currentPath));

			std::cout << "Path Node: " << std::endl;
			std::cout << currentPath;
		}

		// deallocate memory for path offsets
		delete[] m_pathTableNode.pathOffsets;
	}
}

Byaml::~Byaml()
{
	if (m_file.is_open())
	{
		m_file.close();
	}
}
