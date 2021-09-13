#include "ast\fl_ast_decl.h"
#include "parser\fl_parser.h"
#include "job\fl_job.h"
#include "fl_buffer.h"
namespace fluffy { namespace jobs {
	

	/**
	 * Job
	 */

	Job::Job()
		: m_status(JobStatus_e::Idle)
	{}

	Job::~Job()
	{}

	JobStatus_e
	Job::getJobStatus()
	{
		std::lock_guard<std::mutex> guard(m_mutexStatus);
		return m_status;
	}

	void
	Job::setJobStatus(JobStatus_e status)
	{
		std::lock_guard<std::mutex> guard(m_mutexStatus);
		m_status = status;
	}

	const I8*
	Job::getError()
	{
		std::lock_guard<std::mutex> guard(m_mutexError);
		return m_error.c_str();
	}

	void
	Job::setError(const I8* error)
	{
		std::lock_guard<std::mutex> guard(m_mutexError);
		m_error = error;
	}

	/**
	 * JobParseFromSourceFile
	 */

	JobParseFromSourceFile::JobParseFromSourceFile(const I8* sourceFilename)
		: m_sourceFilename(sourceFilename)
	{}

	JobParseFromSourceFile::~JobParseFromSourceFile()
	{}

	void
	JobParseFromSourceFile::doJob()
	{
		parser::ParserContext_s context = parser::ParserContext_s { false };
		auto parser = std::make_unique<parser::Parser>(
			new LazyBuffer()
		);

		try
		{
			parser->loadSourceFromFile(m_sourceFilename);
			m_codeUnit = parser->parseCodeUnit(context);
		}
		catch (std::exception& e)
		{
			setJobStatus(JobStatus_e::Error);
			setError(e.what());
		}
	}

	const I8*
	JobParseFromSourceFile::getSourceFilename()
	{
		return m_sourceFilename;
	}

	std::unique_ptr<ast::CodeUnit>
	JobParseFromSourceFile::getCodeUnit()
	{
		return std::move(m_codeUnit);
	}

	ast::CodeUnit*
	JobParseFromSourceFile::getCodeUnitPointer()
	{
		return m_codeUnit.get();
	}

	/**
	 * JobParseFromSourceBlock
	 */

	JobParseFromSourceBlock::JobParseFromSourceBlock(const I8* sourceFilename, const I8* sourceCode)
		: m_sourceFilename(sourceFilename)
		, m_sourceCode(sourceCode)
	{}

	JobParseFromSourceBlock::~JobParseFromSourceBlock()
	{}

	void
	JobParseFromSourceBlock::doJob()
	{
		parser::ParserContext_s context = parser::ParserContext_s { false };
		auto parser = std::make_unique<parser::Parser>(
			new LazyBuffer()
		);

		try
		{
			parser->loadSource(m_sourceFilename, m_sourceCode);
			m_codeUnit = parser->parseCodeUnit(context);
		}
		catch (std::exception& e)
		{
			setJobStatus(JobStatus_e::Error);
			setError(e.what());
		}
	}

	const I8*
	JobParseFromSourceBlock::getSourceFilename()
	{
		return m_sourceFilename;
	}

	std::unique_ptr<ast::CodeUnit>
	JobParseFromSourceBlock::getCodeUnit()
	{
		return std::move(m_codeUnit);
	}

	ast::CodeUnit*
	JobParseFromSourceBlock::getCodeUnitPointer()
	{
		return m_codeUnit.get();
	}
} }