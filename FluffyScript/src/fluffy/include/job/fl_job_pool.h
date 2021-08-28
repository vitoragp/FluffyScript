#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include "fl_defs.h"
namespace fluffy { namespace jobs {
	class Job;

	enum class JobRunnerStatus_e
	{
		Idle,
		Working,
		Done
	};

	/**
	 * JobRunner
	 */

	class JobRunner
	{
		friend class JobPool;

	public:
		JobRunner();
		~JobRunner();

		void
		close();

		void
		setJob(Job* job);

	private:
		void
		run();

	private:
		Job*
		m_currentJob;

		JobRunnerStatus_e
		m_status;

		std::mutex
		m_mutex;

	};

	/**
	 * JobPool
	 */

	class JobPool
	{
	public:
		JobPool(U32 threadCount);
		~JobPool();

		void
		initialize();

		void
		run();
		
		void
		addJob(Job* job);

		const U32
		getJobCount();

		const U32
		getPendentJobCount();

		const U32
		getWorkingJobCount();

		const U32
		getDoneJobCount();

	private:
		std::vector<std::unique_ptr<JobRunner>>
		m_runnerPool;

		std::vector<std::thread>
		m_threadPool;

		std::vector<Job*>
		m_jobQueue;
	};
} }