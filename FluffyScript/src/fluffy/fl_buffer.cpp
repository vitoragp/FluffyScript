/*
 * fl_buffer.cpp
 *
 *  Created on: 12 de ago. de 2021
 *      Author: NPShinigami
 */

#include <filesystem>
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

	/**
	 * LazyBuffer
	 */

	LazyBuffer::LazyBuffer(const U32 bufferSize)
		: m_memory(nullptr)
		, m_cursor(0)
		, m_length(bufferSize)
		, m_fileSize(0)
		, m_hasCache(false)
	{
		m_memory = reinterpret_cast<I8*>(malloc(bufferSize));
	}

	LazyBuffer::~LazyBuffer()
	{
		if (m_stream.is_open()) {
			m_stream.close();
		}
		if (m_hasCache) {
			std::filesystem::remove(m_cacheFileName);
			std::filesystem::remove(".\\s_cache");
		}
		if (m_memory != nullptr) {
			free(m_memory);
			m_memory = nullptr;
		}
		m_cursor = 0;
		m_length = 0;
		m_hasCache = false;
		m_cacheFileName.clear();
	}

	void LazyBuffer::load(const I8* sourcePtr, const U32 len)
	{
		loadFromFile(cacheSourceFile(sourcePtr, len));
	}

	void LazyBuffer::loadFromFile(const I8* fileName)
	{
		m_stream.open(fileName, std::ifstream::binary);
		if (!m_stream.is_open()) {
			throw exceptions::file_not_found_exception(fileName);
		}

		m_stream.seekg(0, std::fstream::end);
		m_fileSize = m_stream.tellg();
		m_stream.seekg(0, std::fstream::beg);

		// Preenche o buffer inicial
		m_stream.readsome(m_memory, m_length);

	}

	const I8 LazyBuffer::readByte(U8 offset)
	{
		if (offset)
		{
			I8 ch;

			// Se o offset solicitado ja esta no buffer apenas retorna.
			if (m_cursor + offset < m_length)
			{
				return m_memory[m_cursor + offset];
			}

			// Salva a posicao atual.
			size_t cursor = m_stream.tellg();

			// Verifica se o offset solicitado esta dentro do range.
			if (cursor + offset > m_fileSize)
			{
				throw std::out_of_range("Requested offset is out of range.");
			}

			// Avanca ate o offset solicitado, le o byte e volta para a
			// posicao inicial.
			m_stream.seekg(cursor + offset - 1, std::fstream::beg);
			m_stream.readsome(&ch, 1);
			m_stream.seekg(cursor, std::fstream::beg);

			return ch;
		}

		if (m_cursor == m_length)
		{
			if (m_stream.good())
			{
				const U32 totalReadedBytes = m_stream.tellg();
				size_t readedBytes = m_stream.readsome(
					m_memory,
					totalReadedBytes + m_length > m_fileSize
						? m_fileSize - totalReadedBytes
						: m_length
				);
				if (readedBytes < m_length) {
					m_memory[readedBytes] = 0;
				}
				m_cursor = 0;
			}
			else
			{
				throw std::out_of_range("Failed to read source file");
			}
		}
		return m_memory[m_cursor];
	}

	void LazyBuffer::nextByte()
	{
		m_cursor++;
	}

	const I8* LazyBuffer::cacheSourceFile(const I8* sourcePtr, const U32 len)
	{
		static U32 tempFileIndex = 0;
		static I8 fileNameBuffer[64];

		sprintf(fileNameBuffer, ".\\s_cache\\cache_%04d.lex", tempFileIndex++);

		if (!std::filesystem::exists(".\\s_cache")) {
			std::filesystem::create_directories(".\\s_cache");
		}

		std::ofstream outFileStream(fileNameBuffer, std::fstream::binary);
		if (!outFileStream.is_open()) {
			throw exceptions::file_not_found_exception(fileNameBuffer);
		}

		outFileStream.write(sourcePtr, len);
		outFileStream.put('\0');
		outFileStream.close();

		m_hasCache = true;
		m_cacheFileName = fileNameBuffer;

		return m_cacheFileName.c_str();
	}
}



