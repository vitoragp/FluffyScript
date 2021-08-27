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

	U32 DirectBuffer::getPosition()
	{
		return m_cursor;
	}

	const I8 DirectBuffer::readByte(U8 offset)
	{
		return m_memory[m_cursor + offset];
	}

	void DirectBuffer::nextByte()
	{
		m_cursor++;
	}

	void DirectBuffer::reset(U32 position)
	{
		m_cursor = position;
	}

	/**
	 * LazyBuffer
	 */

	LazyBuffer::LazyBuffer(const U32 bufferSize)
		: m_memory(nullptr)
		, m_cursor(0)
		, m_length(bufferSize)
		, m_fileSize(0)
	{
		m_memory = reinterpret_cast<I8*>(malloc(bufferSize));
	}

	LazyBuffer::~LazyBuffer()
	{
		if (m_stream.is_open()) {
			m_stream.close();
		}
		if (m_memory != nullptr) {
			free(m_memory);
			m_memory = nullptr;
		}
		m_cursor = 0;
		m_length = 0;
	}

	void LazyBuffer::load(const I8* sourcePtr, const U32 len)
	{
		throw exceptions::not_implemented_feature_exception("LazyBuffer works with files only");
	}

	void LazyBuffer::loadFromFile(const I8* fileName)
	{
		m_stream.open(fileName, std::ifstream::binary);
		if (!m_stream.is_open()) {
			throw exceptions::file_not_found_exception(fileName);
		}

		m_stream.seekg(0, std::fstream::end);
		m_fileSize = static_cast<U32>(m_stream.tellg());
		m_stream.seekg(0, std::fstream::beg);

		// Preenche o buffer inicial
		m_stream.read(m_memory, m_length);

		// Se o conteudo do arquivo e menor que o buffer
		// implanta no buffer o caractere nulo no final do conteudo.
		if (m_length > m_fileSize)
		{
			m_memory[m_fileSize] = 0;
		}
	}

	U32 LazyBuffer::getPosition()
	{
		return static_cast<U32>(m_stream.tellg()) + m_cursor;
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
			size_t requestedPosition = cursor + offset - 1;

			// Verifica se o offset solicitado esta dentro do range.
			if (requestedPosition > m_fileSize)
			{
				throw std::out_of_range("Requested offset is out of range.");
			}

			// Chegou ao fim do arquivo.
			if (requestedPosition == m_fileSize)
			{
				return 0;
			}

			// Avanca ate o offset solicitado, le o byte e volta para a
			// posicao inicial.
			m_stream.seekg(requestedPosition, std::fstream::beg);
			m_stream.read(&ch, 1);
			m_stream.seekg(cursor, std::fstream::beg);

			return ch;
		}

		if (m_cursor == m_length)
		{
			if (m_stream.good())
			{
				const U32 totalReadedBytes = static_cast<U32>(m_stream.tellg());
				m_stream.read(
					m_memory,
					totalReadedBytes + m_length > m_fileSize
					? m_fileSize - totalReadedBytes
					: m_length
				);
				size_t readedBytes = m_stream.gcount();
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

	void LazyBuffer::reset(U32 position)
	{
		// Vai para a posicao indicada.
		m_stream.seekg(position, std::fstream::beg);

		// Recarrega o buffer.
		if (m_stream.good())
		{
			const U32 totalReadedBytes = static_cast<U32>(m_stream.tellg());
			m_stream.read(
				m_memory,
				totalReadedBytes + m_length > m_fileSize
				? m_fileSize - totalReadedBytes
				: m_length
			);
			size_t readedBytes = m_stream.gcount();
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
}