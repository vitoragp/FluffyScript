#include <iostream>
#include <cstdlib>
#include "ast\fl_ast_decl.h"
#include "parser\fl_parser.h"
#include "job\fl_job_pool.h"
#include "job\fl_job.h"
#include "utils\fl_ast_utils.h"
#include "utils\fl_include_utils.h"
#include "scope\fl_scope_manager.h"
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
		: mScopeManager(new scope::ScopeManager())
		, mJobCount(4)
		, mInitialized(false)
	{}

	Compiler::~Compiler()
	{}

	void
	Compiler::initialize(String basePath)
	{
		mBasePath = basePath;
		mInitialized = true;
	}

	void
	Compiler::build(String sourceFile)
	{
		buildInternal(mBasePath + sourceFile.c_str());
	}

	void
	Compiler::setNumberOfJobs(U32 jobCount)
	{
		if (mInitialized)
		{
			throw exceptions::custom_exception(
				"Job count must be changed before initialize compiler"
			);
		}
		mJobCount = jobCount;
	}

	void
	Compiler::buildInternal(String sourceFile)
	{
		std::unique_ptr<jobs::JobParseFromSourceFile> job;
		{
			// Cria tarefa e enfileira na fila.
			job = std::make_unique<jobs::JobParseFromSourceFile>(sourceFile.c_str());
			job->doJob();

			// Valida os code units por ambiguidades.
			if (job != nullptr)
			{
				if (job->getJobStatus() == jobs::JobStatus_e::Error)
				{
					throw exceptions::custom_exception(job->getError());
				}
			}
		}

		// Junta todos as AST das tarefas.
		ast::CodeUnit* const codeUnit = job->getCodeUnitPointer();

		// TODO: Implementar o processamento de includes em multithread.
		// Processa includes.
		for (auto& include : codeUnit->includeDeclList)
		{
			String fileWithPath;
			if (utils::IncludeUtils::isIncludeFromSystem(include.get()))
			{
				size_t strSize = 0;
				getenv_s(&strSize, nullptr, 0, "FLUFFY_HOME");

				// Verifica se a variavel ambiente 'FLUFFY_HOME' existe.
				if (strSize == 0) {
					throw exceptions::custom_exception("Invalid 'FLUFFY_HOME' environment variable.");
				}

				String str(strSize, 0);
				getenv_s(&strSize, const_cast<char*>(str.c_str()), strSize, "FLUFFY_HOME");

				fileWithPath = str + include->inFile + ".txt";
				throw exceptions::not_implemented_feature_exception("include from system");
			}
			else
			{
				fileWithPath = mBasePath + include->inFile + ".txt";
			}
			buildInternal(fileWithPath);
			
			// Atualiza o nome do arquivo
			include->inFile = fileWithPath;
		}

		// Insere o code unit no arvore de referencia.
		mApplicationTree.emplace(codeUnit->name.c_str(), std::move(job->getCodeUnit()));

		// Insere o code unit na arvore de execucao, essa arvore contem a ordem em que os
		// arquivos foram processados, sendo o primeiro o include mais distante do arquivo fonte de inicio
		// e o ultimo o proprio arquivo de iniciao da aplicação, essa estrutura e importante pois determinara
		// a ordem da validacao e transformacao do codigo.
		mExecutionTree.emplace_back(codeUnit);

		// Insere o code unit no gerenciador de escopo.
		mScopeManager->insertCodeUnit(codeUnit);
	}

}