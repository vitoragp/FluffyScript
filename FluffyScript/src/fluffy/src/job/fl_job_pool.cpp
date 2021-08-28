#include <chrono>
#include "job\fl_job.h"
#include "job\fl_job_pool.h"
namespace fluffy { namespace jobs {
	// Constante de tempo padrao para esperar atualizacoes.
	constexpr const std::chrono::milliseconds sleepTime = std::chrono::milliseconds(5);

	/**
	 * JobRunner
	 */

	JobRunner::JobRunner()
		: m_currentJob(nullptr)
		, m_status(JobRunnerStatus_e::Idle)
	{}

	JobRunner::~JobRunner()
	{}

	void
	JobRunner::close()
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_status = JobRunnerStatus_e::Done;
	}

	void
	JobRunner::setJob(Job* job)
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		{
			if (job)
			{
				m_currentJob = job;
				m_status = JobRunnerStatus_e::Working;
			}
		}
	}

	void
	JobRunner::run()
	{
		using std::chrono::milliseconds;

		while (true)
		{
			// Deixa a thread esperando alguma tarefa.
			while (m_status == JobRunnerStatus_e::Idle)
			{
				std::this_thread::sleep_for(
					milliseconds(sleepTime)
				);
			}

			// Executa a tarefa.
			if (m_currentJob && m_status == JobRunnerStatus_e::Working)
			{
				std::lock_guard<std::mutex> guard(m_mutex);
				{
					m_currentJob->setJobStatus(JobStatus_e::Working);
					m_currentJob->doJob();

					// O estado para feito so e alterado se nao houver erros.
					if (m_currentJob->getJobStatus() == JobStatus_e::Working) {
						m_currentJob->setJobStatus(JobStatus_e::Done);
					}

					m_currentJob = nullptr;
					m_status = JobRunnerStatus_e::Idle;
				}
			}

			// Finaliza a thread.
			if (m_status == JobRunnerStatus_e::Done)
			{
				break;
			}
		}
	}

	/**
	 * JobPool
	 */

	JobPool::JobPool(U32 threadCount)
		: m_runnerPool(threadCount)
		, m_threadPool(threadCount)
	{}

	JobPool::~JobPool()
	{
		// Sinaliza para todos os running serem finalizados.
		for (U32 i = 0; i < m_threadPool.size(); i++)
		{
			m_runnerPool[i]->close();
		}

		// Espera todas as thread serem finalizadas.
		for (auto& t : m_threadPool)
		{
			if (t.joinable()) {
				t.join();
			}
		}
	}

	void
	JobPool::initialize()
	{
		for (U32 i = 0; i < m_threadPool.size(); i++)
		{
			m_runnerPool[i] = std::make_unique<JobRunner>();
			m_threadPool[i] = std::thread(&JobRunner::run, m_runnerPool[i].get());
		}
	}

	void
	JobPool::run()
	{
		while (true)
		{
			Bool hasPendentJob = false;
			for (auto job : m_jobQueue)
			{
				if (job->getJobStatus() == JobStatus_e::Idle)
				{
					hasPendentJob = true;
					for (auto& jobRunner : m_runnerPool)
					{
						jobRunner->setJob(job);
					}
				}
			}

			if (!hasPendentJob)
			{
				break;
			}

			std::this_thread::sleep_for(
				std::chrono::milliseconds(sleepTime)
			);
		}
	}

	void
	JobPool::addJob(Job* job)
	{
		m_jobQueue.push_back(job);
	}

	const U32
	JobPool::getJobCount()
	{
		U32 jobCount = 0;
		for (auto& job : m_jobQueue)
		{
			if (job)
			{
				jobCount++;
			}
		}
		return jobCount;
	}

	const U32
	JobPool::getPendentJobCount()
	{
		U32 jobCount = 0;
		for (auto& job : m_jobQueue)
		{
			if (job->getJobStatus() == JobStatus_e::Idle) {
				jobCount++;
			}
		}
		return jobCount;
	}

	const U32
	JobPool::getWorkingJobCount()
	{
		U32 jobCount = 0;
		for (auto& job : m_jobQueue)
		{
			if (job->getJobStatus() == JobStatus_e::Working) {
				jobCount++;
			}
		}
		return jobCount;
	}

	const U32
	JobPool::getDoneJobCount()
	{
		U32 jobCount = 0;
		for (auto& job : m_jobQueue)
		{
			if (job->getJobStatus() == JobStatus_e::Done) {
				jobCount++;
			}
		}
		return jobCount;
	}
} }