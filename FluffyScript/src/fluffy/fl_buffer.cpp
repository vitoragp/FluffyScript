/*
 * fl_buffer.cpp
 *
 *  Created on: 12 de ago. de 2021
 *      Author: NPShinigami
 */

#include <fstream>
#include "fl_buffer.h"
#include "fl_exceptions.h"

namespace fluffy {
	/**
	 * BufferBase
	 */

	BufferBase::BufferBase()
	{}

	BufferBase::~BufferBase()
	{}

	/**
	 * DirectBuffer
	 */

	DirectBuffer::DirectBuffer()
		: m_memory(nullptr)
		, m_cursor(0)
		, m_length(0)
	{}

	DirectBuffer::~DirectBuffer()
	{
		if (m_memory != nullptr) {
			free(m_memory);
			m_memory = nullptr;
		}
		m_cursor = 0;
		m_length = 0;
	}

	void DirectBuffer::load(const I8* sourcePtr, const U32 len)
	{
		if (!m_memory) {
			m_memory = reinterpret_cast<I8*>(malloc(len + 1));
			m_memory[len] = 0;
		}
		memcpy(m_memory, sourcePtr, len);
	}

	void DirectBuffer::loadFromFile(const I8* fileName)
	{
		std::ifstream fileStream(fileName, std::ifstream::binary);
		if (!fileStream.is_open()) {
			throw exceptions::file_not_found_exception(fileName);
		}

		// Calcula tamanho do arquivo.
		fileStream.seekg(0, std::ifstream::end);
		size_t len = fileStream.tellg();
		fileStream.seekg(0, std::ifstream::beg);

		if (!m_memory) {
			m_memory = reinterpret_cast<I8*>(malloc(len + 1));
			m_memory[len] = 0;
		}

		// Copia o conteudo do arquivo para a string.
		fileStream.read(m_memory, len);

		fileStream.close();
	}

	const I8 DirectBuffer::readByte(U8 offset)
	{
		return m_memory[m_cursor + offset];
	}

	void DirectBuffer::nextByte()
	{
		m_cursor++;
	}
}



