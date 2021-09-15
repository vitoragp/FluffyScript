#pragma once
#include <exception>
#include <vector>
#include <string>
#include "fl_defs.h"

namespace fluffy {
	namespace exceptions {
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
									unexpected_token_exception(const String& filename, I8 token, U32 line, U32 column);
									unexpected_token_exception(const String& filename, String token, U32 line, U32 column);
									~unexpected_token_exception();

			virtual const char*		what() const noexcept override;

		private:
			String					m_filename;
			Bool					m_isChar;
			I8 						m_tokenChar;
			String					m_tokenString;
			U32 					m_line;
			U32 					m_column;
		};

		/**
		 * unexpected_with_possibilities_token_exception
		 */

		class unexpected_with_possibilities_token_exception : public std::exception
		{
		public:
									unexpected_with_possibilities_token_exception(const String& filename, String token, std::vector<TokenType_e> possibilities, U32 line, U32 column);
									~unexpected_with_possibilities_token_exception();

			virtual const char*		what() const noexcept override;

		private:
			String					m_filename;
			String					m_out;
			U32 					m_line;
			U32 					m_column;
		};

		/**
		 * unexpected_end_of_file_exception
		 */

		class unexpected_end_of_file_exception : public std::exception
		{
		public:
									unexpected_end_of_file_exception(const String& filename);
									~unexpected_end_of_file_exception();

			virtual const char*		what() const noexcept override;

		private:
			String					m_filename;
		};

		/**
		 * malformed_number_exception
		 */

		class malformed_number_exception : public std::exception
		{
		public:
									malformed_number_exception(const String& filename, U32 line, U32 column);
									~malformed_number_exception();

			virtual const char*		what() const noexcept override;

		private:
			String					m_filename;
			U32 					m_line;
			U32 					m_column;
		};

		/**
		 * malformed_character_constant_exception
		 */

		class malformed_character_constant_exception : public std::exception
		{
		public:
									malformed_character_constant_exception(const String& filename, U32 line, U32 column);
									~malformed_character_constant_exception();

			virtual const char*		what() const noexcept override;

		private:
			String					m_filename;
			U32 					m_line;
			U32 					m_column;
		};

		/**
		 * malformed_string_constant_exception
		 */

		class malformed_string_constant_exception : public std::exception
		{
		public:
									malformed_string_constant_exception(const String& filename, U32 line, U32 column);
									~malformed_string_constant_exception();

			virtual const char*		what() const noexcept override;

		private:
			String					m_filename;
			U32 					m_line;
			U32 					m_column;
		};

		/**
		 * not_implemented_feature_exception
		 */

		class not_implemented_feature_exception : public std::exception
		{
		public:
									not_implemented_feature_exception(const String& filename, String feature);
									~not_implemented_feature_exception();

			virtual const char*		what() const noexcept override;

		private:
			String					m_filename;
			String 					m_feature;
		};

		/**
		 * unexpected_type_exception
		 */

		class unexpected_type_exception : public std::exception
		{
		public:
									unexpected_type_exception(const String& filename, String type, U32 line, U32 column);
									~unexpected_type_exception();

			virtual const char*		what() const noexcept override;

		private:
			String					m_filename;
			const U32 				m_line;
			const U32 				m_column;
			const String			m_type;
		};

		/**
		 * expected_type_exception
		 */

		class expected_type_exception : public std::exception
		{
		public:
									expected_type_exception(const String& filename, U32 line, U32 column);
									~expected_type_exception();

			virtual const char*		what() const noexcept override;

		private:
			String					m_filename;
			const U32 				m_line;
			const U32 				m_column;
		};

		/**
		 * custom_exception
		 */

		class custom_exception : public std::exception
		{
		public:
									custom_exception(const I8* message, U32 line, U32 column, ...);
									custom_exception(const I8* message, ...);
									~custom_exception();

			virtual const char*		what() const noexcept override;

		private:
			String					m_message;
			const Bool				m_hasPositionalInfo;
			const U32 				m_line;
			const U32 				m_column;
		};
	}
}