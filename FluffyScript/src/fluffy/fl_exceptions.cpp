#include <cstdarg>
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

	const char* file_not_found_exception::what() const noexcept
	{
		static char buffer[256];
		sprintf_s(buffer, "File not found: '%s'", m_filename.c_str());
		return buffer;
	}

	/**
		* unexpected_token_exception
		*/

	unexpected_token_exception::unexpected_token_exception(I8 token, U32 line, U32 column)
		: m_isChar(true)
		, m_tokenChar(token)
		, m_line(line)
		, m_column(column)
	{}

	unexpected_token_exception::unexpected_token_exception(String token, U32 line, U32 column)
		: m_isChar(false)
		, m_tokenString(token)
		, m_line(line)
		, m_column(column)
	{}

	unexpected_token_exception::~unexpected_token_exception()
	{}

	const char* unexpected_token_exception::what() const noexcept
	{
		static char buffer[256];
		if (m_isChar) {
			sprintf_s(buffer, "Unexpected token '%c' at: line %d, column %d", m_tokenChar, m_line, m_column);
		} else {
			sprintf_s(buffer, "Unexpected token '%s' at: line %d, column %d", m_tokenString.c_str(), m_line, m_column);
		}
		return buffer;
	}

	/**
		* unexpected_end_of_file_exception
		*/

	unexpected_end_of_file_exception::unexpected_end_of_file_exception()
	{}

	unexpected_end_of_file_exception::~unexpected_end_of_file_exception()
	{}

	const char* unexpected_end_of_file_exception::what() const noexcept
	{
		static char buffer[256];
		sprintf_s(buffer, "Unexpected end of file");
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

	const char* malformed_number_exception::what() const noexcept
	{
		static char buffer[256];
		sprintf_s(buffer, "Malphormed number at: line %d, column %d", m_line, m_column);
		return buffer;
	}

	/**
		* malformed_character_constant_exception
		*/

	malformed_character_constant_exception::malformed_character_constant_exception(U32 line, U32 column)
		: m_line(line)
		, m_column(column)
	{}

	malformed_character_constant_exception::~malformed_character_constant_exception()
	{}

	const char* malformed_character_constant_exception::what() const noexcept
	{
		static char buffer[256];
		sprintf_s(buffer, "Malphormed character constant at: line %d, column %d", m_line, m_column);
		return buffer;
	}

	/**
		* malformed_string_constant_exception
		*/

	malformed_string_constant_exception::malformed_string_constant_exception(U32 line, U32 column)
		: m_line(line)
		, m_column(column)
	{}

	malformed_string_constant_exception::~malformed_string_constant_exception()
	{}

	const char* malformed_string_constant_exception::what() const noexcept
	{
		static char buffer[256];
		sprintf_s(buffer, "Malphormed string constant at: line %d, column %d", m_line, m_column);
		return buffer;
	}

	/**
		* not_implemented_feature_exception
		*/

	not_implemented_feature_exception::not_implemented_feature_exception(String feature)
		: m_feature(feature)
	{}

	not_implemented_feature_exception::~not_implemented_feature_exception()
	{}

	const char* not_implemented_feature_exception::what() const noexcept
	{
		static char buffer[256];
		sprintf_s(buffer, "this feature '%s' is not implemented", m_feature.c_str());
		return buffer;
	}

	/**
	 * unexpected_type_exception
	 */

	unexpected_type_exception::unexpected_type_exception(String type, U32 line, U32 column)
		: m_line(line)
		, m_column(column)
		, m_type(type)
	{}

	unexpected_type_exception::~unexpected_type_exception()
	{}

	const char* unexpected_type_exception::what() const noexcept
	{
		static char buffer[256];
		sprintf_s(buffer, "Unexpected type '%s' at: line %d, column %d", m_type.c_str(), m_line, m_column);
		return buffer;
	}

	/**
	 * expected_type_exception
	 */

	expected_type_exception::expected_type_exception(U32 line, U32 column)
		: m_line(line)
		, m_column(column)
	{}

	expected_type_exception::~expected_type_exception()
	{}

	const char* expected_type_exception::what() const noexcept
	{
		static char buffer[256];
		sprintf_s(buffer, "Expected type declaration at: line %d, column %d", m_line, m_column);
		return buffer;
	}

	/**
	 * custom_exception
	 */

	custom_exception::custom_exception(String message, U32 line, U32 column, ...)
		: m_message()
		, m_line(line)
		, m_column(column)
	{
		static char buffer[512];
		va_list list;

		va_start(list, column);
		vsprintf_s(buffer, 512, message.c_str(), list);
		va_end(list);

		m_message = buffer;
	}

	custom_exception::~custom_exception()
	{}

	const char* custom_exception::what() const noexcept
	{
		static char buffer[600];
		sprintf_s(buffer, "%s at: line %d, column %d", m_message.c_str(), m_line, m_column);
		return buffer;
	}
} }