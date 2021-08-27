#pragma once
#include <memory>
#include "fl_defs.h"

namespace fluffy { namespace parser {
	class Parser;
} }

namespace fluffy {
	/**
	 * Compiler
	 */

	class Compiler
	{
	public:
		Compiler();
		virtual ~Compiler();

		void
		loadSource(const I8* sourceCode);

		void
		loadSourceFromFile(const I8* sourceCodeFilename);

		void
		build();

	private:
		std::unique_ptr<parser::Parser>
		m_parser;
	};
}