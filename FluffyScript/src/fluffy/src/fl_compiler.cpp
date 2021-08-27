#include "parser\fl_parser.h"
#include "fl_buffer.h"
#include "fl_compiler.h"
namespace fluffy {
	// Tamanho padrao do buffer: 16KB.
	static constexpr const U32 bufferSize = 16384;

	/**
	 * Compiler
	 */
	
	Compiler::Compiler()
		: m_parser(new parser::Parser(new LazyBuffer(bufferSize)))
	{}

	Compiler::~Compiler()
	{}

	void
	Compiler::loadSource(const I8* sourceCode)
	{
		m_parser->loadSource(sourceCode);
	}

	void
	Compiler::loadSourceFromFile(const I8* sourceCodeFilename)
	{
		m_parser->loadSourceFromFile(sourceCodeFilename);
	}

	void
	Compiler::build()
	{}
}