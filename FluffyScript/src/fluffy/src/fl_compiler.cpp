#include <iostream>
#include "ast\fl_ast_decl.h"
#include "parser\fl_parser.h"
#include "validation\fl_validate_duplication.h"
#include "job\fl_job_pool.h"
#include "job\fl_job.h"
#include "utils\fl_ast_utils.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"
#include "fl_compiler.h"
namespace fluffy {
	// Tamanho padrao do buffer: 16KB.
	static constexpr const U32 bufferSize = 16384;

	/**
	 * Compiler
	 */
	
	Compiler::Compiler()
		: m_parser(new parser::Parser(new LazyBuffer(bufferSize)))
		, m_jobCount(4)
		, m_initialized(false)
	{}

	Compiler::~Compiler()
	{}

	void
	Compiler::initialize()
	{
		m_initialized = true;
	}

	void
	Compiler::buildMultiThread(std::vector<String> sourceFileList)
	{
		std::unordered_map<String, std::unique_ptr<jobs::Job>> sourceJobMap;
		{
			auto jobPool = std::make_unique<jobs::JobPool>(4);

			jobPool->initialize();

			// Cria tarefa e enfileira na fila.
			for (auto& sourceFile : sourceFileList)
			{
				auto job = std::make_unique<jobs::JobParseFromSourceFile>(sourceFile.c_str());
				jobPool->addJob(job.get());
				sourceJobMap.insert(std::pair<String, std::unique_ptr<jobs::Job>>(sourceFile, std::move(job)));
			}

			// Executa todas as tarefas em fila.
			jobPool->run();

			// Valida os code units por ambiguidades.
			for (auto& job : sourceJobMap)
			{
				if (job.second->getJobStatus() == jobs::JobStatus_e::Error)
				{
					throw exceptions::custom_exception(job.second->getError());
				}

				if (auto jobParse = reinterpret_cast<jobs::JobParseFromSourceFile*>(job.second.get()))
				{
					if (jobParse->getCodeUnitPointer())
					{
						auto validation = validations::DuplicatedValidation();
						validation.validate(jobParse->getCodeUnitPointer());
					}
				}
			}
		}

		// Verifica o estado das tarefas.
		for (auto& sourceJob : sourceJobMap)
		{
			if (sourceJob.second->getJobStatus() == jobs::JobStatus_e::Error) {
				throw exceptions::custom_exception(
					"%s: error %s", sourceJob.first.c_str(), sourceJob.second->getError()
				);
			}
		}

		// Junta todos as AST das tarefas.
		for (auto& sourceJob : sourceJobMap)
		{
			if (auto job = dynamic_cast<jobs::JobParseFromSourceFile*>(sourceJob.second.get()))
			{
				auto codeUnit = job->getCodeUnit();

				// Valida a duplicacao de elementos individualmente em cada code unit.
				auto validateDuplication = std::make_unique<validations::DuplicatedValidation>();
				validateDuplication->validate(codeUnit.get());

				// Insere o code unit no rootscope
				m_rootScope.insert(
					std::pair<const I8*, std::unique_ptr<ast::CodeUnit>>(
						codeUnit->name.c_str(), std::move(codeUnit)
					)
				);
			}
		}
	}

	void
	Compiler::buildSingleThread(std::vector<String> sourceFileList)
	{
		std::unordered_map<String, std::unique_ptr<jobs::Job>> sourceJobMap;
		{
			// Cria tarefa e enfileira na fila.
			for (auto& sourceFile : sourceFileList)
			{
				auto job = std::make_unique<jobs::JobParseFromSourceFile>(sourceFile.c_str());
				job->doJob();
				sourceJobMap.insert(std::pair<String, std::unique_ptr<jobs::Job>>(sourceFile, std::move(job)));
			}

			// Valida os code units por ambiguidades.
			for (auto& job : sourceJobMap)
			{
				if (job.second->getJobStatus() == jobs::JobStatus_e::Error)
				{
					throw exceptions::custom_exception(job.second->getError());
				}

				if (auto jobParse = reinterpret_cast<jobs::JobParseFromSourceFile*>(job.second.get()))
				{
					if (jobParse->getCodeUnitPointer())
					{
						auto validation = validations::DuplicatedValidation();
						validation.validate(jobParse->getCodeUnitPointer());
					}
				}
			}
		}

		// Verifica o estado das tarefas.
		for (auto& sourceJob : sourceJobMap)
		{
			if (sourceJob.second->getJobStatus() == jobs::JobStatus_e::Error) {
				throw exceptions::custom_exception(
					"%s: error %s", sourceJob.first.c_str(), sourceJob.second->getError()
				);
			}
		}

		// Junta todos as AST das tarefas.
		for (auto& sourceJob : sourceJobMap)
		{
			if (auto job = dynamic_cast<jobs::JobParseFromSourceFile*>(sourceJob.second.get()))
			{
				auto codeUnit = job->getCodeUnit();

				// Valida a duplicacao de elementos individualmente em cada code unit.
				auto validateDuplication = std::make_unique<validations::DuplicatedValidation>();
				validateDuplication->validate(codeUnit.get());

				// Insere o code unit no rootscope
				m_rootScope.insert(
					std::pair<const I8*, std::unique_ptr<ast::CodeUnit>>(
						codeUnit->name.c_str(), std::move(codeUnit)
					)
				);
			}
		}
	}

	void
	Compiler::setNumberOfJobs(U32 jobCount)
	{
		if (m_initialized)
		{
			throw exceptions::custom_exception(
				"Job count must be changed before initialize compiler"
			);
		}
		m_jobCount = jobCount;
	}
}