/*
 * fl_exceptions.h
 *
 *  Created on: 11 de ago. de 2021
 *      Author: NPShinigami
 */

#ifndef FL_EXCEPTIONS_H_
#define FL_EXCEPTIONS_H_
#include <exception>
#include "fl_defs.h"

namespace fluffy { namespace exceptions {
	/**
	 *
	 */

	class file_not_found_exception : public std::exception
	{
	public:
								file_not_found_exception(const String filename);
								~file_not_found_exception();

		virtual const char*		what() const noexcept override;

	private:
		String					m_filename;
	};

	/**
	 * unexpected_token_exception
	 */

	class unexpected_token_exception : public std::exception
	{
	public:
								unexpected_token_exception(I8 token, U32 line, U32 column);
								~unexpected_token_exception();

		virtual const char*		what() const noexcept override;

	private:
		I8 						m_token;
		U32 					m_line;
		U32 					m_column;
	};

	/**
	 * unexpected_end_of_file_exception
	 */

	class unexpected_end_of_file_exception : public std::exception
	{
	public:
								unexpected_end_of_file_exception();
								~unexpected_end_of_file_exception();

		virtual const char*		what() const noexcept override;
	};

	/**
	 * malformed_number_exception
	 */

	class malformed_number_exception : public std::exception
	{
	public:
								malformed_number_exception(U32 line, U32 column);
								~malformed_number_exception();

		virtual const char*		what() const noexcept override;

	private:
		U32 					m_line;
		U32 					m_column;
	};

	/**
	 * malformed_character_constant_exception
	 */

	class malformed_character_constant_exception : public std::exception
	{
	public:
								malformed_character_constant_exception(U32 line, U32 column);
								~malformed_character_constant_exception();

		virtual const char*		what() const noexcept override;

	private:
		U32 					m_line;
		U32 					m_column;
	};

	/**
	 * malformed_string_constant_exception
	 */

	class malformed_string_constant_exception : public std::exception
	{
	public:
								malformed_string_constant_exception(U32 line, U32 column);
								~malformed_string_constant_exception();

		virtual const char*		what() const noexcept override;

	private:
		U32 					m_line;
		U32 					m_column;
	};
} }
#endif /* FL_EXCEPTIONS_H_ */
