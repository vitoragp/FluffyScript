/*
 * fl_exceptions.cpp
 *
 *  Created on: 11 de ago. de 2021
 *      Author: NPShinigami
 */

#include "fl_exceptions.h"

namespace fluffy { namespace exceptions {
	/**
	 * file_not_found_exception
	 */

	file_not_found_exception::file_not_found_exception(const String filename)
		: m_filename(filename.c_str())
	{}

	file_not_found_exception::~file_not_found_exception()
	{}

	const char* file_not_found_exception::what() noexcept
	{
		static char buffer[256];
		sprintf(buffer, "File not found: '%s'", m_filename.c_str());
		return buffer;
	}

	/**
	 * unexpected_token_exception
	 */

	unexpected_token_exception::unexpected_token_exception(U32 line, U32 column)
		: m_line(line)
		, m_column(column)
	{}

	unexpected_token_exception::~unexpected_token_exception()
	{}

	const char* unexpected_token_exception::what() noexcept
	{
		static char buffer[256];
		sprintf(buffer, "Unexpected token at: line %d, column %d", m_line, m_column);
		return buffer;
	}

	/**
	 * unexpected_end_of_file_exception
	 */

	unexpected_end_of_file_exception::unexpected_end_of_file_exception()
	{}

	unexpected_end_of_file_exception::~unexpected_end_of_file_exception()
	{}

	const char* unexpected_end_of_file_exception::what() noexcept
	{
		static char buffer[256];
		sprintf(buffer, "Unexpected end of file");
		return buffer;
	}

	/**
	 * malformed_number_exception
	 */

	malformed_number_exception::malformed_number_exception(U32 line, U32 column)
		: m_line(line)
		, m_column(column)
	{}

	malformed_number_exception::~malformed_number_exception()
	{}

	const char* malformed_number_exception::what() noexcept
	{
		static char buffer[256];
		sprintf(buffer, "Malphormed number at: line %d, column %d", m_line, m_column);
		return buffer;
	}
} }

