#pragma once
#include <memory>
#include <mutex>
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class CodeUnit;
} }

namespace fluffy { namespace jobs {
	/**
	 * JobStatus_e
	 */

	enum class JobStatus_e
	{
		Idle,
		Working,
		Done,
		Error
	};

	/**
	 * Job
	 */

	class Job
	{
	public:
		Job();
		virtual ~Job();

		virtual void
		doJob() = 0;

		JobStatus_e
		getJobStatus();

		void
		setJobStatus(JobStatus_e status);

		const I8*
		getError();

		void
		setError(const I8* error);

	private:
		std::mutex
		m_mutexStatus;

		std::mutex
		m_mutexError;

		JobStatus_e
		m_status;

		String
		m_error;
	};

	/**
	 * JobParseFromSourceFile
	 */

	class JobParseFromSourceFile final : public Job
	{
	public:
		JobParseFromSourceFile(const I8* sourceFilename);
		virtual ~JobParseFromSourceFile();

		virtual void
		doJob() override;

		const I8*
		getSourceFilename();

		std::unique_ptr<ast::CodeUnit>
		getCodeUnit();

		ast::CodeUnit*
		getCodeUnitPointer();

	private:
		const I8*
		m_sourceFilename;

		std::unique_ptr<ast::CodeUnit>
		m_codeUnit;
	};

	/**
	 * JobParseFromSourceBlock
	 */

	class JobParseFromSourceBlock final : public Job
	{
	public:
		JobParseFromSourceBlock(const I8* sourceFilename, const I8* sourceCode);
		virtual ~JobParseFromSourceBlock();

		virtual void
		doJob() override;

		const I8*
		getSourceFilename();

		std::unique_ptr<ast::CodeUnit>
		getCodeUnit();

		ast::CodeUnit*
		getCodeUnitPointer();

	private:
		const I8*
		m_sourceFilename;

		const I8*
		m_sourceCode;

		std::unique_ptr<ast::CodeUnit>
		m_codeUnit;
	};

} }