#pragma once
#include <memory>
#include "fl_buffer.h"
#include "fl_defs.h"

namespace fluffy {
	namespace lexer {
		/**
		 * Lexer
		 */

		class Lexer
		{
		private:
			using BufferBasePtr = std::unique_ptr<BufferBase>;

		public:
			Lexer(BufferBase* const buffer);
			~Lexer();

			void 					loadSource(String source);
			void					loadFromSource(String sourceFile);

			void 					parse(Token_s& tok);

			void					setTabSpaces(U32 newTabSpaces);

		private:
			I8 						readChar(U32 offset = 0);
			I8 						readCharAndAdv();
			void					nextChar();
			void					skip();

			void					parseIdentifier(Token_s& tok);
			void					parseSymbols(Token_s& tok);
			void					parseNumbers(Token_s& tok);
			void					parseChar(Token_s& tok);
			void					parseString(Token_s& tok);

		private:
			BufferBasePtr			m_buffer;
			U32 					m_line;
			U32 					m_column;
			U32						m_tabSpaces;
			Bool					m_eof;
			String					m_filename;
		};
	}
}

