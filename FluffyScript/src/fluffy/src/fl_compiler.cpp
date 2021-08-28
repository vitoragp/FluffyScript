#include <iostream>
#include "ast\fl_ast_decl.h"
#include "parser\fl_parser.h"
#include "validation\fl_validate_structure.h"
#include "job\fl_job_pool.h"
#include "job\fl_job.h"
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
	Compiler::build(std::vector<String> sourceFileList)
	{
		std::unordered_map<String, std::unique_ptr<jobs::Job>> sourceJobMap;
		{
			auto jobPool = std::make_unique<jobs::JobPool>(1);

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
				auto codeUnit = std::move(job->getCodeUnit());

				// Percorre os namespaces.
				for (auto&& namespaceDecl : codeUnit->namespaceDeclList)
				{
					mergeOrInsertNamespaceToRoot(codeUnit->name.c_str(), std::move(namespaceDecl));
				}
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

	void
	Compiler::mergeOrInsertNamespaceToRoot(const I8* sourceFilename, std::unique_ptr<ast::NamespaceDecl> nsDecl)
	{
		auto nsIt = m_rootScope.find(nsDecl->identifier.str());

		if (nsIt == m_rootScope.end())
		{
			m_rootScope.insert(std::pair<const I8*, std::unique_ptr<ast::NamespaceDecl>>(
				nsDecl->identifier.str(), std::move(nsDecl))
			);
		}
		else
		{
			for (auto&& nsChild : nsDecl->namespaceDeclList)
			{
				mergeOrInsertChildNamespace(sourceFilename, nsIt->second.get(), std::move(nsChild));
			}

			for (auto&& geSrcChild : nsDecl->generalDeclList)
			{
				for (auto&& geDstChild : nsIt->second->generalDeclList)
				{
					if (geSrcChild->identifier == geDstChild->identifier)
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier: '%s' at: line %d, column %d",
							sourceFilename,
							geSrcChild->identifier.str(),
							geSrcChild->line,
							geSrcChild->column
						);
					}
				}
				nsIt->second->generalDeclList.push_back(std::move(geSrcChild));
			}
		}
	}

	void
	Compiler::mergeOrInsertChildNamespace(const I8* sourceFilename, ast::NamespaceDecl* destinationNS, std::unique_ptr<ast::NamespaceDecl> sourceNs)
	{
		for (auto& nsSrcChild : sourceNs->namespaceDeclList)
		{
			Bool foundedNamespace = false;
			for (auto& nsDstChild : destinationNS->namespaceDeclList)
			{
				if (nsSrcChild->identifier == nsDstChild->identifier)
				{
					foundedNamespace = true;
					mergeOrInsertChildNamespace(sourceFilename, nsDstChild.get(), std::move(nsSrcChild));
					break;
				}
			}
			if (!foundedNamespace)
			{
				destinationNS->namespaceDeclList.push_back(std::move(nsSrcChild));
			}
		}

		for (auto& geSrcChild : sourceNs->generalDeclList)
		{
			for (auto& geDstChild : destinationNS->namespaceDeclList)
			{
				if (geSrcChild->identifier == geDstChild->identifier)
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier: '%s' at: line %d, column %d",
						sourceFilename,
						geSrcChild->identifier.str(),
						geSrcChild->line,
						geSrcChild->column
					);
				}
			}
			destinationNS->generalDeclList.push_back(std::move(geSrcChild));
		}
	}
}